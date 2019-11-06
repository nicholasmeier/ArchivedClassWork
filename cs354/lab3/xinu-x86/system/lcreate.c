/*	lcreate.c - lcreate	*/
#include <xinu.h>


/* Lab 3: lcreate - create a new lock, return the id to the caller */

local int32 newlck(void);

int32 lcreate() {
	intmask mask;
	int32 lck; /* Lock Id to return */
	//Disable mask during any modification of global vars
	mask = disable();
	
	
	if ( ( lck=newlck() )==SYSERR ){
		restore(mask); 
		return SYSERR;
	}
		
	
	restore(mask);

	return lck;
	
}

local int32 newlck(void){
	int32 i = 0;
	
	for (i = 0; i<NLOCKS; i++){
		if ( (locktab[i].lstate == L_FREE) || (locktab[i].lstate == DELETED) ){
			locktab[i].lstate = L_USED;
			locktab[i].luid = lockcnt++;
			return i;
		}
	}
	return SYSERR;

}
