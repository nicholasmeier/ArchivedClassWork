#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "myMalloc.h"
#include "printing.h"

/* Due to the way assert() prints error messges we use out own assert function
 * for deteminism when testing assertions
 */
#ifdef TEST_ASSERT
  inline static void assert(int e) {
    if (!e) {
      fprintf(stderr, "Assertion Failed!\n");
      exit(1);
    }
  }
#else
  #include <assert.h>
#endif

/*
 * Mutex to ensure thread safety for the freelist
 */
static pthread_mutex_t mutex;

/*
 * Array of sentinel nodes for the freelists
 */
header freelistSentinels[N_LISTS];

/*
 * Pointer to the second fencepost in the most recently allocated chunk from
 * the OS. Used for coalescing chunks
 */
header * lastFencePost;

/*
 * Pointer to maintian the base of the heap to allow printing based on the
 * distance from the base of the heap
 */ 
void * base;

/*
 * List of chunks allocated by  the OS for printing boundary tags
 */
header * osChunkList [MAX_OS_CHUNKS];
size_t numOsChunks = 0;

/*
 * Used to store a history of what was allocated
 * Used for finding left_size, coalescing freed blocks, etc
 */
header * lastAlloc = NULL;
header * lastFree = NULL;
int numBlocksAlloc = 0;

/*
 * direct the compiler to run the init function before running main
 * this allows initialization of required globals
 */
static void init (void) __attribute__ ((constructor));

// Helper functions for manipulating pointers to headers
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off);
static inline header * get_left_header(header * h);
static inline header * ptr_to_header(void * p);

// Helper functions for allocating more memory from the OS
static inline void initialize_fencepost(header * fp, size_t left_size);
static inline void insert_os_chunk(header * hdr);
static inline void insert_fenceposts(void * raw_mem, size_t size);
static header * allocate_chunk(size_t size);

// Helper functions for freeing a block
static inline void deallocate_object(void * p);

// Helper functions for allocating a block
static inline header * allocate_object(size_t raw_size);

// Custum Helper function for removing fenceposts for new chunks
static inline void deallocate_fenceposts(header * fp1);

// Helper functions for verifying that the data structures are structurally 
// valid
static inline header * detect_cycles();
static inline header * verify_pointers();
static inline bool verify_freelist();
static inline header * verify_chunk(header * chunk);
static inline bool verify_tags();

// Library initialization
static void init();

/**
 * @brief Helper function to retrieve a header pointer from a pointer and an 
 *        offset
 *
 * @param ptr base pointer
 * @param off number of bytes from base pointer where header is located
 *
 * @return a pointer to a header offset bytes from pointer
 */
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off) {
  return (header *)((char *) ptr + off);
}

/**
 * @brief Helper function to get the header to the right of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
header * get_right_header(header * h) {
  return get_header_from_offset(h, h->size);
}

/**
 * @brief Helper function to get the header to the left of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
inline static header * get_left_header(header * h) {
  return get_header_from_offset(h, -h->left_size);
}

/**
 * @brief Fenceposts are marked as always allocated and may need to have
 * a left object size to ensure coalescing happens properly
 *
 * @param fp a pointer to the header being used as a fencepost
 * @param left_size the size of the object to the left of the fencepost
 */
inline static void initialize_fencepost(header * fp, size_t left_size) {
  fp->allocated = FENCEPOST;
  fp->size = ALLOC_HEADER_SIZE;
  fp->left_size = left_size;
}

/**
 * @brief Helper function to maintain list of chunks from the OS for debugging
 *
 * @param hdr the first fencepost in the chunk allocated by the OS
 */
inline static void insert_os_chunk(header * hdr) {
  if (numOsChunks < MAX_OS_CHUNKS) {
    osChunkList[numOsChunks++] = hdr;
  }
}

/**
 * @brief given a chunk of memory insert fenceposts at the left and 
 * right boundaries of the block to prevent coalescing outside of the
 * block
 *
 * @param raw_mem a void pointer to the memory chunk to initialize
 * @param size the size of the allocated chunk
 */
inline static void insert_fenceposts(void * raw_mem, size_t size) {
  // Convert to char * before performing operations
  char * mem = (char *) raw_mem;

  // Insert a fencepost at the left edge of the block
  header * leftFencePost = (header *) mem;
  initialize_fencepost(leftFencePost, ALLOC_HEADER_SIZE);

  // Insert a fencepost at the right edge of the block
  header * rightFencePost = get_header_from_offset(mem, size - ALLOC_HEADER_SIZE);
  initialize_fencepost(rightFencePost, size - 2 * ALLOC_HEADER_SIZE);
}

/**
 * @brief Allocate another chunk from the OS and prepare to insert it
 * into the free list
 *
 * @param size The size to allocate from the OS
 *
 * @return A pointer to the allocable block in the chunk (just after the 
 * first fencpost)
 */
static header * allocate_chunk(size_t size) {
  void * mem = sbrk(size);
  if (mem == NULL || size == -1 || mem == -1){ return NULL; } 
  insert_fenceposts(mem, size);
  header * hdr = (header *) ((char *)mem + ALLOC_HEADER_SIZE);
  hdr->allocated = UNALLOCATED;
  hdr->size = size - 2 * ALLOC_HEADER_SIZE;
  hdr->left_size = ALLOC_HEADER_SIZE;
  return hdr;
}

/**
 * @brief Helper allocate an object given a raw request size from the user
 *
 * @param raw_size number of bytes the user needs
 *
 * @return A block satisfying the user's request
 */
static inline header * allocate_object(size_t raw_size) {
	if (raw_size <= 0){
		return (header*)NULL;
	}
	size_t actual_size = 0;


	//rounds raw_size up to the nearest multipe of the minimum allocation ammount	
	if ((raw_size % MIN_ALLOCATION) != 0){
		actual_size = MIN_ALLOCATION + raw_size - (raw_size % MIN_ALLOCATION) + ALLOC_HEADER_SIZE;
	}else{
		actual_size = raw_size + ALLOC_HEADER_SIZE;
	}
	if (actual_size < sizeof(header)){
		actual_size = sizeof(header);
	}
	//set up the vars for the for loop
	size_t t;
	header * block;
	//use as starting index for the freelists, prevent looking at too small freelists
	int i = ((actual_size - ALLOC_HEADER_SIZE)/8)-1; 
	if (i > (N_LISTS-1)){i = N_LISTS-1;}

	for (; i < N_LISTS; i++){
		header * freelist = &freelistSentinels[i];
		block = freelist->next;
		while(block != freelist){ //iterate through the freelist
			t = block->size;
			if (actual_size == t){ //allocated mem is same size as block
				block->allocated = ALLOCATED; 
				block->size = actual_size;
				freelist->next = block->next;
				freelist->prev = block->prev;
				//numBlocksAlloc++;
				return  (header*) (block->data);
				//entire block is allocated
			}
			if (actual_size < t){ //check that the allcated mem will fit in the block
				if ((t-actual_size)%8 == 0){ //prevents excess useless memory
					header * temp = get_header_from_offset(block, t - actual_size);
					temp->size = actual_size;
					block->size = t - actual_size; //uses part of the block
					int ni = ((block->size-ALLOC_HEADER_SIZE)/8)-1;
					if (ni > (N_LISTS-1)){ni = N_LISTS-1;}
					//if the block isn't in the same free list, then change lists
					if (ni != i){
						header * newlist = &freelistSentinels[ni];
						freelist->next = block->next;
						freelist->prev = block->prev;
						if (newlist->next != newlist){
							header * e = newlist->next;
							block->next = e;
							e->prev = block;
							block->prev = newlist;
							newlist->next = block;
						}else{
							newlist->next = block;
							newlist->prev = block;
							block->next = newlist;
							block->prev = newlist;
						}
					}
					//set the details of the allocated block
					temp->left_size = block->size;
					temp->allocated = ALLOCATED;
					
					//set the right block's left size to the allocated block
					header * rightBlock = get_right_header(temp);
					rightBlock->left_size = temp->size;

					return (header*)(temp->data);
				}
			}
			block = block->next; //used to iterate through freelists
		}
		
		if (i == N_LISTS-1){ //if no blocks satisfy the request then ask for more memory 
			header * new_block = allocate_chunk(ARENA_SIZE);
			header * prevFencePost = get_header_from_offset(new_block, -ALLOC_HEADER_SIZE);
			header * oldlast = lastFencePost;
			lastFencePost = get_header_from_offset(new_block, new_block->size);
			header * zzz = get_left_header(new_block);
			header * old_fp = get_left_header(zzz);
			if (new_block == NULL) { return NULL;}
			t = new_block->size;
			if (actual_size == t){ //size of new block is size of request
				new_block->allocated = ALLOCATED;
				if (oldlast == old_fp){ //if the new chunk is adjacent to old, coalesce
					deallocate_fenceposts(old_fp);
				}else{
					insert_os_chunk(prevFencePost);
				}
				return (header*) (new_block->data);
			}
			if (actual_size < t){ //size of new block > size of request
				//does similar stuff to the code if there was a block in other freelists
				header * temp = get_header_from_offset(new_block, t - actual_size);
				temp->size = actual_size;
				new_block->size = t - actual_size;
				int ni = ((new_block->size-ALLOC_HEADER_SIZE)/8)-1;
				if (ni > (N_LISTS-1)){ni = N_LISTS-1;}
				header * newlist = &freelistSentinels[ni];
				if (newlist->next != newlist){
					header * e = newlist->next;
					new_block->next = e;
					e->next = newlist;
					newlist->prev = e;
					e->prev = new_block;
					new_block->prev = newlist;
					newlist->next = new_block;
				}else{
					newlist->next = new_block;
					newlist->prev = new_block;
					new_block->next = newlist;
					new_block->prev = newlist;
				}
				header * rightBlock = get_right_header(temp);
				temp->left_size = new_block->size;
				temp->allocated = ALLOCATED;
				rightBlock->left_size = actual_size;
				if (oldlast == old_fp){	//if new chunk == adjacent to old, coalesce
					deallocate_fenceposts(old_fp);
				}else{
					insert_os_chunk(prevFencePost);
				}
				return (header*)(temp->data);
			}
			
		}
	}
	return NULL; //None of the conditions were satisfied, something must have gone wrong, re NULL
}

/**
 * @brief Helper to get the header from a pointer allocated with malloc
 *
 * @param p pointer to the data region of the block
 *
 * @return A pointer to the header of the block
 */
static inline header * ptr_to_header(void * p) {
  return (header *)((char *) p - ALLOC_HEADER_SIZE); //sizeof(header));
}

/**
 * @brief Helper to manage deallocation of a pointer returned by the user
 *
 * @param p The pointer returned to the user by a call to malloc
 */
static inline void deallocate_object(void * p) {
	if (p == NULL){ //NULL pointer --> no-op
		return ;
	}

	header * block = ptr_to_header(p);
	header * freelist;

	if (block->allocated == UNALLOCATED){ //user fucked up
		printf("Double Free Detected\nAssertion Failed!\n");
		exit(1);
	}

	int ind = 0; //index for freelists
	
	header * leftBlock = get_left_header(block);
	header * rightBlock = get_right_header(block);
	header * rrBlock = get_right_header(rightBlock);
	header * newList = NULL;

	if (leftBlock->allocated == UNALLOCATED){ //handle coalescing blocks (left)
		int oldSize = leftBlock->size;
		leftBlock->size += block->size;
		block->allocated = UNALLOCATED;
		if (rightBlock->allocated == UNALLOCATED){ //handle coalescing blocks (right/left)
			leftBlock->size += rightBlock->size;
			rrBlock->left_size = leftBlock->size;
			int indOld = ((rightBlock->size - ALLOC_HEADER_SIZE)/8)-1;
			if (indOld > (N_LISTS-1)){ indOld = N_LISTS-1; }
			header * oldList = &freelistSentinels[indOld];
			if (rightBlock->next = oldList){
				oldList->next = rightBlock->next;
				oldList->prev = rightBlock->prev;
			}
		}else{
			rightBlock->left_size = leftBlock->size;
		}
		if ( (((oldSize-ALLOC_HEADER_SIZE)/8)-1) >= (N_LISTS-1) ){
			return;	
		}
		ind = ((leftBlock->size - ALLOC_HEADER_SIZE)/8)-1;
		if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
		newList = &freelistSentinels[ind];
		leftBlock->next = newList->next;
		leftBlock->prev = newList;
		newList->next = leftBlock;
		return ;
	}

	if (rightBlock->allocated == UNALLOCATED){ //handle coalescing blocks (right)
		block->allocated = UNALLOCATED;
		block->size += rightBlock->size;
		rrBlock->left_size = block->size;
		int indOld = ((rightBlock->size - ALLOC_HEADER_SIZE)/8)-1;
		if (indOld > (N_LISTS-1)){indOld = N_LISTS-1; }
		header * oldList = &freelistSentinels[indOld];
		//delete rightblock from its old freelist
		if (rightBlock->next == oldList){
			oldList->next = rightBlock->next;
			oldList->prev = rightBlock->prev;
		}
		//set up new freelist position
		ind = ((block->size - ALLOC_HEADER_SIZE)/8)-1;
		if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
		newList = &freelistSentinels[ind];
		if (newList->next != newList){
			header * e = newList->next;
			block->next = e;
			e->prev = block;
			newList->next = block;
			block->prev = newList->prev;
		}else{
			newList->next = block;
			block->next = newList;
			newList->prev = block;
			block->prev = newList;
		}
		return;
	}
	//none of the adjacent blocks are also free, just free this block
	ind = ((block->size - ALLOC_HEADER_SIZE)/8)-1;
	if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
	freelist = &freelistSentinels[ind];
	rightBlock->left_size = block->size;
	
	if (freelist->next != freelist){
		header * e = freelist->next;
		block->next = e;
		e->prev = block;
		block->prev = freelist;
		freelist->next = block;
	}else{
		freelist->next = block;
		block->next = freelist;
		freelist->prev = block;
		block->prev = freelist;
	}
	block->allocated = UNALLOCATED; //we allocated now bishes
}

/** Hey look, I made this
 * @brief Helper to manage the removal of fenceposts when new os chunks are added
 * basically took a version of deallocate_object and fucked it up just enough to make this
 *
 * @param block, the header of a fencepost
 */
static inline void deallocate_fenceposts(header * fp1) {
	if (fp1 == NULL){
		return ;
	}
	/* How this shit is organized
	 *	[ BLOCK1 ] Old Chunk
	 *	[  FP1   ]
	 *	[  FP2   ] New Chunk, allocated by OS
	 *	[ BLOCK2 ] 
	 *	[ BLOCK3 ]
	 */
	header * freelist;
	if (fp1->allocated == UNALLOCATED){
		printf("Thats not supposed to happen\n");
	}
	int ind = 0;
	header * block1 = get_left_header(fp1); //old leftBlock
	header * fp2 = get_right_header(fp1); //old rightBlock
	header * block2 = get_right_header(fp2); //old rrBlock
	header * block3 = get_right_header(block2); //old rrrBlock
	header * newList = NULL;
	if (block1->allocated == UNALLOCATED){
		int oldSize = block1->size;
		block1->size += fp1->size + fp2->size;
		fp1->allocated = UNALLOCATED;
		fp2->allocated = UNALLOCATED;
		if (block2->allocated == UNALLOCATED){
			block1->size += block2->size;
			block3->left_size = block1->size;
			int indOld = ((block2->size - ALLOC_HEADER_SIZE)/8)-1;
			if (indOld > (N_LISTS-1)){ indOld = N_LISTS-1; }
			header * oldList = &freelistSentinels[indOld];
			if (block2->next == oldList){
				oldList->next = oldList;
				oldList->prev = oldList;
			}
		}else{
			block2->left_size = block1->size;
		}
		if ( (((oldSize-ALLOC_HEADER_SIZE)/8)-1) >= (N_LISTS-1) ){
			return;	
		}
		int indOld = ((oldSize - ALLOC_HEADER_SIZE)/8)-1;
		if (indOld > (N_LISTS-1)){ indOld = N_LISTS-1; }
		header * oldList = &freelistSentinels[indOld];
		oldList->next = block1->next;
		oldList->prev = block1->prev;
		ind = ((block1->size - ALLOC_HEADER_SIZE)/8)-1;
		if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
		newList = &freelistSentinels[ind];
		if (newList->next != newList){
			header * e = newList->next;
			e->next = newList;
			block1->next = e;
			block1->prev = newList;
			e->prev = block1;
			newList->prev = e;
			newList->next = block1;
		}else{
			block1->next = newList;
			block1->prev = newList;
			newList->next = block1;
			newList->prev = block1;
		}
		return ;
	}
	if (block2->allocated == UNALLOCATED){
		fp1->allocated = UNALLOCATED;
		fp2->allocated = UNALLOCATED;
		fp1->size += fp2->size + block2->size;
		block3->left_size = fp1->size;
		int indOld = ((block2->size - ALLOC_HEADER_SIZE)/8)-1;
		if (indOld > (N_LISTS-1)){indOld = N_LISTS-1; }
		header * oldList = &freelistSentinels[indOld];
		if (block2->next == oldList){
			oldList->next = block2->next;
			oldList->prev = block2->prev;
		}	
		ind = ((fp1->size - ALLOC_HEADER_SIZE)/8)-1;
		if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
		newList = &freelistSentinels[ind];
		if (newList->next != newList){
			header * e = newList->next;
			fp1->next = e;
			e->prev = fp1;
			newList->next = fp1;
			fp1->prev = newList->prev;
		}else{
			newList->next = fp1;
			fp1->next = newList;
			newList->prev = fp1;
			fp1->prev = newList;
		}
		return;
	}
	fp1->size += fp2->size;
	ind = ((fp1->size - ALLOC_HEADER_SIZE)/8)-1;
	if (ind > (N_LISTS-1)){ ind = N_LISTS-1; }
	freelist = &freelistSentinels[ind];
	block2->left_size = fp1->size;
	
	if (freelist->next != freelist){
		header * e = freelist->next;
		e->next = freelist;
		fp1->next = e;
		e->prev = fp1;
		fp1->prev = freelist;
		freelist->next = fp1;
		freelist->prev = e;
	}else{
		freelist->next = fp1;
		fp1->next = freelist;
		freelist->prev = fp1;
		fp1->prev = freelist;
	}
	fp1->allocated = UNALLOCATED;
	fp2->allocated = UNALLOCATED;
}


/**
 * @brief Helper to detect cycles in the free list
 * https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_Tortoise_and_Hare
 *
 * @return One of the nodes in the cycle or NULL if no cycle is present
 */
static inline header * detect_cycles() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * slow = freelist->next, * fast = freelist->next->next; 
         fast != freelist; 
         slow = slow->next, fast = fast->next->next) {
      if (slow == fast) {
        return slow;
      }
    }
  }
  return NULL;
}

/**
 * @brief Helper to verify that there are no unlinked previous or next pointers
 *        in the free list
 *
 * @return A node whose previous and next pointers are incorrect or NULL if no
 *         such node exists
 */
static inline header * verify_pointers() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * cur = freelist->next; cur != freelist; cur = cur->next) {
      if (cur->next->prev != cur || cur->prev->next != cur) {
        return cur;
      }
    }
  }
  return NULL;
}

/**
 * @brief Verify the structure of the free list is correct by checkin for 
 *        cycles and misdirected pointers
 *
 * @return true if the list is valid
 */
static inline bool verify_freelist() {
  header * cycle = detect_cycles();
  if (cycle != NULL) {
    fprintf(stderr, "Cycle Detected\n");
    print_sublist(print_object, cycle->next, cycle);
    return false;
  }

  header * invalid = verify_pointers();
  if (invalid != NULL) {
    fprintf(stderr, "Invalid pointers\n");
    print_object(invalid);
    return false;
  }

  return true;
}

/**
 * @brief Helper to verify that the sizes in a chunk from the OS are correct
 *        and that allocated node's canary values are correct
 *
 * @param chunk AREA_SIZE chunk allocated from the OS
 *
 * @return a pointer to an invalid header or NULL if all header's are valid
 */
static inline header * verify_chunk(header * chunk) {
  if (chunk->allocated != FENCEPOST) {
    fprintf(stderr, "Invalid fencepost\n");
    print_object(chunk);
    return chunk;
  }

  for (; chunk->allocated != FENCEPOST; chunk = get_right_header(chunk)) {
    if (chunk->size  != get_right_header(chunk)->left_size) {
      fprintf(stderr, "Invalid sizes\n");
      print_object(chunk);
      return chunk;
    }
  }

  return NULL;
}

/**
 * @brief For each chunk allocated by the OS verify that the boundary tags
 *        are consistent
 *
 * @return true if the boundary tags are valid
 */
static inline bool verify_tags() {
  for (size_t i = 0; i < numOsChunks; i++) {
    header * invalid = verify_chunk(osChunkList[i]);
    if (invalid != NULL) {
      return invalid;
    }
  }

  return NULL;
}

/**
 * @brief Initialize mutex lock and prepare an initial chunk of memory for allocation
 */
static void init() {
  // Initialize mutex for thread safety
  pthread_mutex_init(&mutex, NULL);

#ifdef DEBUG
  // Manually set printf buffer so it won't call malloc when debugging the allocator
  setvbuf(stdout, NULL, _IONBF, 0);
#endif // DEBUG

  // Allocate the first chunk from the OS
  header * block = allocate_chunk(ARENA_SIZE);

  header * prevFencePost = get_header_from_offset(block, -ALLOC_HEADER_SIZE);
  insert_os_chunk(prevFencePost);

  lastFencePost = get_header_from_offset(block, block->size);

  // Set the base pointer to the beginning of the first fencepost in the first
  // chunk from the OS
  base = ((char *) block) - ALLOC_HEADER_SIZE; //sizeof(header);

  // Initialize freelist sentinels
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    freelist->next = freelist;
    freelist->prev = freelist;
  }

  // Insert first chunk into the free list
  header * freelist = &freelistSentinels[N_LISTS - 1];
  freelist->next = block;
  freelist->prev = block;
  block->next = freelist;
  block->prev = freelist;
}

/* 
 * External interface
 */
void * my_malloc(size_t size) {
  pthread_mutex_lock(&mutex);
  header * hdr = allocate_object(size); 
  pthread_mutex_unlock(&mutex);
  return hdr;
}

void * my_calloc(size_t nmemb, size_t size) {
  return memset(my_malloc(size * nmemb), 0, size * nmemb);
}

void * my_realloc(void * ptr, size_t size) {
  void * mem = my_malloc(size);
  memcpy(mem, ptr, size);
  my_free(ptr);
  return mem; 
}

void my_free(void * p) {
  pthread_mutex_lock(&mutex);
  deallocate_object(p);
  pthread_mutex_unlock(&mutex);
}

bool verify() {
  return verify_freelist() && verify_tags();
}
