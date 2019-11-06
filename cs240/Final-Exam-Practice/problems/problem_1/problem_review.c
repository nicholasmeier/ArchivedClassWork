#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "review.h"

// Problem (1/4)
/******************************************************************************
 * TODO: Return a pointer to the first occurrence of the character 'c' in the 
 * string 's'. Return NULL if the character is not found.
 * 
 * Parameters: s -- string
 *             c -- search for this character in the string 's'
 *
 * Return: pointer to the first occurene of the character c in the string s
 *
 * Return Type: char pointer
 *****************************************************************************/
char *strchr(const char *s, int c) 
{
	// Write Your Code Here
	int x = 0;	
	while(s[x] != '\0'){
		if (s[x] == c){
			return &(s[x]);
		}
		x++;
	}
	return NULL;
}

// Problem (2/4)
/******************************************************************************
 * TODO: Return the maximum element in the array
 * 
 * Parameters: array -- an integer array
 *             n -- number of elements in the array
 *
 * Return: the maximum element in the array
 *
 * Return Type: integer
 *****************************************************************************/
int max_array( int * array, int n )
{
	// Write Your Code Here
	int x;
	int max;
	for (x = 0; x < n; x++){
		if (x == 0){
			max = array[x];
		}
		if (array[x] > max){
			max = array[x];
		}
	}
	return max;
}

// Problem (3/4)
/******************************************************************************
 * TODO: Return the 'ith' bit in 'bitmap' (either 0 or 1)
 * 
 * Parameters: bitmap -- unsigned integer
 *             ith -- index of the bit to be returned
 *
 * Return: the largest element in the array 
 *
 * Return Type: integer
 *****************************************************************************/
int get_bit_at( unsigned int bitmap, int ith )
{
	unsigned int s = sizeof(unsigned int);
	unsigned int mp = 1<<(s*8-1);
	int x;
	unsigned int ar[(s*8-1)];
	for (x = 0; x < s*8; x++){
		ar[x] = (bitmap&mp ? 1 : 0);
		bitmap = bitmap<<1;
	}
	return ar[31-ith];
}

// Problem (4/4)
/******************************************************************************
 * TODO: Read from the file and store the entries in to a single linked list.
 * Return the list. Use the function llist_add( List * list, int value ) to
 * add entries to the list. 
 *
 * The file format will be:
 * value\n
 * value\n
 * value\n
 * ...
 *
 * Parameters: filename -- the name of the file you will be reading from
 *
 * Return: a singled linked list which contains the values read from the file
 *
 * Return Type: pointer to a single linked list
 *****************************************************************************/
List * llist_read( char * filename ) 
{

	List * listA = malloc(sizeof(List));
	
	llist_init(listA);
	
	FILE * fd;
	fd = fopen(filename, "r");
	int c;
	int x;
	char cas[sizeof(int)]; 
	int y = 0;
	int z;
	for (z = 0; z < sizeof(int); z++){
		cas[z] = '\0';
	}
	while((c = fgetc(fd))!=EOF){
		if (c == '\n'){
			sscanf(cas, "%d", &x);
			llist_add(listA, x);
			for(z = 0; z < sizeof(int); z++){
				cas[z] = '\0';
			}
			y = 0;
		}else{
			cas[y] = c;
			y++;
		}
	}
	return listA;	
}
