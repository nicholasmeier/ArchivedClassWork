/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/

	//lab 4 code
	pid32 s_pid;
	if (nonempty(prptr->sendqueue)){
		s_pid = dequeue(prptr->sendqueue);
		//struct procent *s_prptr = &proctab[s_pid];
		ready(s_pid); /* Ready the first blocked process for next recieve*/
	}
	
	restore(mask);
	return msg;
}
