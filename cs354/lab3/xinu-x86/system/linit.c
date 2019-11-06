/*	linit.c	- linit	initialize lock system */
#include <xinu.h>

/* Lab 3: Complete this function */

// declare any global variables here
struct lockent locktab[NLOCKS];
int32 lockcnt;


void linit(void) { /* Called in initialize.c */
	int32 i;
	struct lockent *lockptr; /* Pointer to the lock table */
	lockcnt = 0;
	for(i = 0; i < NLOCKS; i++){
		/* Initialize all the locks in the locktable */
		lockptr = &locktab[i];
		lockptr->lstate = L_FREE;
		lockptr->lcnt = 1;
		lockptr->lqueue = newqueue();
		lockptr->lprio = -6969; /*Unused?*/
		lockptr->luid = -1; /* Set all ids to -1 to indicate no use yet*/
	}
} 
