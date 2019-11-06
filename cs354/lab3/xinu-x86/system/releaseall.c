/*	releaseall.c - releaseall	*/
#include <xinu.h>
#include <stdarg.h>

/* Lab 3: The Equivelent of signal for locks*/

syscall releaseall (int32 numlocks, ...) {

	va_list args;
	va_start(args, numlocks);

	intmask mask;
	struct lockent *lckptr;
	int32 ldes;

	mask = disable();
	for (int32 i = 0; i < numlocks; i++){
		ldes = va_arg(args, int32);
		if(isbadlock(ldes)){ /* if one is bad assume all must be bad */
			restore(mask);
			return SYSERR;
		}
		lckptr = &locktab[ldes];

		/* Lock nonexistant conditionals */
		if (lckptr->lstate == L_FREE){  
			restore(mask);
			return SYSERR; 
		}
		if (lckptr->lstate == DELETED){
			restore(mask);
			return DELETED;
		}

		/*Release waiting process*/
		//Should probably change to deal with multiple readers per lock but eh
		if ((lckptr->lcnt++) < 0) {
			ready(dequeue(lckptr->lqueue));
		}
		
	}
	va_end(args);

	return OK;
}
