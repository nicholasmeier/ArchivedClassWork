/* r_mem_allocated.c - r_mem_allocated - LAB5 */

#include <xinu.h>

/*---------------------------------------------------------------------
 * r_mem_allocated - store 
 *---------------------------------------------------------------------
 */
syscall r_mem_allocated(
		pid32 pid,  	/* Process the allocated mem belongs to 		       */
		uint32* buf	/* Location to write to the amount of mem belonging to process */
	)
{
	intmask mask; //Not sure if need to disable interupts yet
	mask = disable();

	if (isbadpid(pid)){
		restore(mask);
		return SYSERR;
	}
	
	//store how much mem is allocated - how much memory is freed (store in proc stuffs?)
	//access this value and return it here, store in buf provided
	
	*buf = (&proctab[pid])->prmembytes;
	restore(mask);
	return OK;	
}
