/* sendblk.c */

#include <xinu.h>

/*------------------------------------------------------------------------------------------
 * LAB 4 CODE - NICHOLAS MEIER
 * sendblk - Pass message to process, if reciever's buffer is full, set sending proc to wait
 *-------------------------------------------------------------------------------------------
 */ 

syscall sendblk(pid32 pid, umsg32 msg){
	intmask mask;
	struct procent *r_prptr;
	struct procent *c_prptr;
	
	mask = disable();
	if(isbadpid(pid)){
		restore(mask);
		return SYSERR;
	}
	
	r_prptr = &proctab[pid];
	c_prptr = &proctab[currpid]; /* process pointer for current process */

	if (r_prptr->prstate == PR_FREE){
		restore(mask);
		return SYSERR;
	}


	/* Blocking functionality */
	if (r_prptr->prhasmsg == TRUE){ /* Recieveing process msg buffer is full*/
		
		//Reciever process tags
		r_prptr->rcpblkflag = TRUE; /* recieving flag says there are things in sendqueue*/
		enqueue(currpid, r_prptr->sendqueue); /*add current process id to sendqueue*/
		
		//Sending process tags
		c_prptr->prstate = PR_SNDBLK; /* set state to wait, specific to blocked send*/
		c_prptr->sendblkflag = TRUE; /* sending process is blocked, msg is stored*/
		c_prptr->sendblkmsg = msg; /* stored message */
		resched();
		
		if (r_prptr->prstate == PR_FREE){ /* Reciever process has been killed */
			//make sure everything is reset to initial value
			r_prptr->rcpblkflag = FALSE; 
			while(!isempty(r_prptr->sendqueue)){
				dequeue(r_prptr->sendqueue);
			}
			c_prptr->sendblkflag = FALSE;
			c_prptr->sendblkmsg = 0; // dunno if this is valid?
			//Dunno if we need to send syserr? probably should
			//restore(mask);
			//return SYSERR;
		} 
	}
	
	/* Code to make this syscall work like send */	
	r_prptr->prmsg = msg;
	r_prptr->prhasmsg = TRUE;
	/*If recipient waiting or in timed-wait make it ready */
	if (r_prptr->prstate == PR_RECV){
		ready(pid);
	}else if (r_prptr->prstate == PR_RECTIM){
		unsleep(pid);
		ready(pid);
	}
	restore(mask);
	return OK;
	
}
