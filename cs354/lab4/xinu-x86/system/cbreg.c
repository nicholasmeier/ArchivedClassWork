/* cbreg.c - cbreg */

#include <xinu.h>

/*-------------------------------------------------------------------
 * cbreg - register a callback function for stuff and things??? idk
 *-------------------------------------------------------------------
 */

syscall cbreg( int (* fnp)(void)){
	struct procent *prptr;
	intmask mask;

	mask = disable();
	
	if (fnp == NULL){ /* Nullpointer handle */ 
		restore(mask);
		return SYSERR;
	}
	/* Set proc vars to match function */
	prptr = &proctab[currpid];
	if (prptr->prhascb){ /* Can't have 2 cb functions */
		restore(mask);
		return SYSERR;
	}
	prptr->prhascb = TRUE;
	prptr->fptr = fnp;

	restore(mask);
	return OK;
}
