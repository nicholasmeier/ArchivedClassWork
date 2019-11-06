/*  lock.h	*/

#ifndef NLOCKS
#define NLOCKS 50 /* Number of locks, if not already defined */
#endif

/* Used for ldelete() */
#define DELETED (-2) /* Lock has been deleted, when waiting on del. lock, return this const. */

/* Used for lcreate()*/
#define L_FREE 0 /*Entry in table is free*/
#define L_USED 1 /*Entry in table is being used*/
#define READ 2 /* can be shared with other readers */
#define WRITE 3 /* Must be exclusive lock */

struct	lockent {
	int32 lstate; /* Entry is deleted, free, created(notype), created(type = read or write) */
	int32 lcnt; /* not sure if need this*/
	qid16 lqueue; /* processes waiting on lock*/
	int32 lprio; /* For priority of last process*/ /*probably unused*/
	int32 luid; /* Unique id for deletion handling */	
};



/*Incompleted Tasks*/
//Extra Credit, Priority inheritance

/* Lab 3 lock table */
extern struct lockent locktab[NLOCKS];
extern int32 lockcnt;

/* Mirror the isbadsem() function from semaphore's implementation */
#define isbadlock(l)	((int32)(l) < 0 || (l) >= NLOCKS)
