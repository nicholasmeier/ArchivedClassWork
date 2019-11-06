/*	lock.c - lock */
#include <xinu.h>

/* Lab 3: Lock the *ock away! */

syscall lock(int32 ldes, int32 type, int32 lpriority) {
	
		
	struct lockent *lckptr; /* Lock table entry */
	struct procent *prptr; /* Process table entry */
	
	intmask mask;
	mask = disable();

	if(isbadlock(ldes)){
		restore(mask);
		return SYSERR;
	}
	if( (type != READ) && (type != WRITE) ){ /* invalid type*/
		restore(mask);
		return SYSERR;
	}

	lckptr = &locktab[ldes];
	if (lckptr->lstate == L_FREE){
		restore(mask);
		return SYSERR;
	}
	
	if (lckptr->lstate == DELETED){ /*Extra precaution for deleted locks */
		/*If lstate is DELETED, then new lcreate has not been called for this lock*/
		restore(mask);
		return DELETED;
	}

	//Readers never wait unless writers has lock, or writer with higher priority is already waiting
	//When writer / last reader releases lock, should give lock to prio with next highest prio

	prptr = &proctab[currpid];

	/* Lock has been deleted, waiting process needs to know this */
	if ( (prptr->lockid != -1) && (prptr->lockid != lckptr->luid)){
		restore(mask);
		return DELETED;
	}
	prptr->locktype = type;
	
	/* Locks taken by Write processes are exclusive : no other holding process*/
	if ((lckptr->lstate == WRITE) && (--(lckptr->lcnt) < 0)){
		prptr->prstate = PR_WAIT;
		prptr->prsem = ldes;
		prptr->lockid = lckptr->luid;
		//enqueue(currpid, lckptr->lqueue);
		insert(currpid, lckptr->lqueue, lpriority);
		resched();
		return OK;
	}
	if ((lckptr->lstate == READ) && (type == READ)){
		pid32 temp = getfirst(lckptr->lqueue);
		struct procent *tmpptr = &proctab[temp];
		if (tmpptr->locktype == WRITE){
			if (tmpptr->lockprio > lpriority){
				prptr->prstate = PR_WAIT;
				prptr->prsem = ldes;
				prptr->lockid = lckptr->luid;
				insert(currpid, lckptr->lqueue, lpriority);
				resched();
				restore(mask);
				return OK;
			}
		}
		//allow read to continue?
		//maybe check priorities
	}

	//Old semaphore implements
	if (--(lckptr->lcnt) < 0){ /* Lock is locked, curr process wait on proc in locked area */
		prptr->prstate = PR_WAIT; /* Set currpid to wait */
		prptr->prsem = ldes; /* Save lock as sem? */ 
		prptr->lockid = lckptr->luid;
		insert(currpid, lckptr->lqueue, lpriority); /* enqueue process in lock's queue*/
		resched(); //Not sure if we should do something else instead of resched?
	}else{
		lckptr->lprio = lpriority;
		prptr->lockid = -1;
		/* process is finished with the lock*/
	}
	
	restore(mask);
	return OK;
}
