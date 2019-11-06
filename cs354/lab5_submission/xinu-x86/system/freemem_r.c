/* freemem_r.c - freemem_r - LAB5 */

#include <xinu.h>

/*------------------------------------------------------------------
 * freemem_r - Function like freemem, but handle garbage collection
 *------------------------------------------------------------------
 */
syscall freemem_r(
		char 	*blkaddr,	/* Pointer to mem block	  */
		uint32 	nbytes		/* Size of block in bytes */
	)
{
	intmask mask;
	struct memblk *next, *prev, *block;
	uint32 top;

	struct  usedblk *u_prev, *u_curr, *u_block;
	struct  procent *prptr; 

	mask = disable(); //Disable interupts

	//ensure that size of block is not invalid
	if((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			 || ((uint32) blkaddr > (uint32) maxheap)){
		restore(mask);
		return SYSERR;
	}

	uint32 t_b = nbytes;
	
	nbytes = (uint32) roundmb(nbytes);
	block = (struct memblk *)blkaddr;
	
	prev = &memlist;
	next = memlist.mnext;
	while((next != NULL) && (next < block)){
		prev = next;
		next = next->mnext;
	}
	
	if (prev == &memlist){ /* compute top of prev block */
		top = (uint32) NULL;
	}else{
		top = (uint32) prev + prev->mlength;
	}

	/* ensure new block doesn't overlap adjacent blocks */
	if (((prev != &memlist) && (uint32) block < top)
		|| ((next != NULL) && (uint32) block+nbytes > (uint32)next)){
		restore(mask);	
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/* remove entry from process's usedlist*/
	prptr = &proctab[currpid];
	u_prev = &(prptr->prusedlist);
	u_curr = (prptr->prusedlist).unext;
	u_block = (struct usedblk *)blkaddr;
	while ( (u_curr != NULL) && (u_curr != u_block) ){
		u_prev = u_curr;
		u_curr = u_curr->unext;
	}
	
	prptr->prmembytes -= t_b;
	u_prev->unext = u_curr->unext;
	
	/* Coalesce with prev block or add to free list */

	if (top == (uint32) block){ /* Coalesce with prev block */
		prev->mlength += nbytes;
		block = prev;
	}else { /* Link into free list as new node */
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}
	
	/* Coalesce with next block if adjacent */
	
	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}
	restore(mask);
	return OK;
}	
