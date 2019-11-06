/*	ldelete.c - ldelete 	*/
#include <xinu.h>

/* Lab 3: Complete this function */

syscall ldelete( 
		int32 ldes	/* lock descriptor */
	)
{
	intmask mask;
	struct lockent *lckptr;
	mask = disable();
	
	if (isbadlock(ldes)) { /* lock id is bad */
		restore(mask);
		return SYSERR;
	}
	
	lckptr = &locktab[ldes];

	/* lock at given id isn't in use */
	if ((lckptr->lstate == L_FREE)||(lckptr->lstate == DELETED)){ 
		restore(mask);
		return SYSERR;
	}

	//Set state to deleted then idk how this is different than just setting it to free? idk
	lckptr->lstate = DELETED;
	
	//TODO The "elegant" deletion part
	//Don't ready procs waiting on this lock, instead have those procs return DELETED
	resched_cntl(DEFER_START);
	while((lckptr->lcnt++) < 0) {
		ready(getfirst(lckptr->lqueue)); //do this?
		//Make processes ready, and return to the deleted lock
		//If the lock is newly created, then compare ids, see that old id != new id
		//return deleted
	}
	resched_cntl(DEFER_STOP);

	restore(mask);
	return OK;
}
