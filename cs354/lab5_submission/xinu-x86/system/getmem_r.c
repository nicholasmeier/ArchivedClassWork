/* getmem_r.c - getmem_r - LAB5 */

#include <xinu.h>
/*-------------------------------------------------------------------
 * getmem_r - function like getmem, but handle garbage collection
 *-------------------------------------------------------------------
 */
char *getmem_r(
		uint32 nbytes	/* Size of memory requested */
	)
{
	intmask mask;
	struct memblk *prev, *curr, *leftover;
	struct usedblk *u_prev, *u_curr;

	mask = disable();
	if (nbytes == 0){
		restore(mask);
		return (char *)SYSERR;
	}
	
	uint32 t_b = nbytes;	
	nbytes = (uint32)roundmb(nbytes);
	
	struct procent *prptr = &proctab[currpid];
	u_prev = &(prptr->prusedlist);
	u_curr = (prptr->prusedlist).unext;

	prev = &memlist;
	curr = memlist.mnext;
	while(curr != NULL){ /* iterate through freelist */
		if (curr->mlength == nbytes) { /* Exact match */
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

			//store allocated ptr in proctab
			prptr->prmembytes += t_b;
			while(u_curr != NULL){ //get to end of usedlist
				u_prev = u_curr;
				u_curr = u_curr->unext;
			}
			u_curr = (struct usedblk *)curr;
			u_curr->ulength = nbytes;

			restore(mask);
			return (char *)(curr);	
		}else if( curr->mlength > nbytes){ /* split big boye*/
			leftover = (struct memblk *)((uint32) curr + nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			
			//store allocated ptr in proctab
			prptr->prmembytes += t_b;
			while(u_curr != NULL){ //get to end of usedlist
				u_prev = u_curr;
				u_curr = u_curr->unext;
			}
			u_curr = (struct usedblk *)curr;
			u_curr->ulength = nbytes;
			
			restore(mask);
			return (char *)(curr);
		}else{ /* move to next block */
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR; //Found no block big enough
}
