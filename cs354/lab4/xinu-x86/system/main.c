/*  main.c  - LAB 4 SAMPLE TEST CASE */

#include <xinu.h>
#include <stdio.h>

void receiver(void) {
	umsg32 msgbuf;
	sleep(2);
	while (TRUE) {
		msgbuf = receive();
		kprintf("Receiver Process %d, Message: %d\n", currpid, msgbuf);
	}
}

int32 receiver_callback(void) {
	umsg32 msgbuf;
	msgbuf = receive();
	kprintf("In callback of Process %d, Message: %d\n", currpid, msgbuf);
	return OK;
}

int32 sender(pid32 p1, umsg32 msg){
//	kprintf("Sending \"%d\" to Process %d\n", msg, p1);
	int32 ret = sendblk(p1, msg);
	if(ret != OK)
		kprintf("FAILED Sending %d\n", msg);
	sleep(4);
	return OK;
}

int32 sender_no_blk(pid32 p1, umsg32 msg){
//	kprintf("Sending \"%d\" to Process %d\n", msg, p1);
	int32 ret = send(p1, msg);
	if(ret != OK)
		kprintf("FAILED Sending %d\n", msg);
	sleep(4);
	return OK;
}

int32 test_reg_cb(void) {
	int32 ret = cbreg(&receiver_callback); 
	if(ret != OK)
		kprintf("Registering CB failed\n");

	while(1)
		;

	return OK;
}

process	main(void)
{
	kprintf("\nSample Tests\n");
	pid32 p1;
	pid32 p2;

	kprintf("\nTest 0: Send (No blocking)\n");
	p1 = create(receiver, 1024, 10, "receiver", 0);
	resume(create(sender_no_blk, 2048, 10, "sender1-1", 2, p1, 11));
	resume(create(sender_no_blk, 2048, 10, "sender1-2", 2, p1, 12));
	resume(create(sender_no_blk, 2048, 10, "sender1-3", 2, p1, 13));
	sleepms(500); 
	resume(p1);
	sleep(5);
	kill(p1);
	
	kprintf("\nTest 1: Send-blocking\n");
	p1 = create(receiver, 1024, 10, "receiver", 0);
	resume(create(sender, 2048, 10, "sender2-1", 2, p1, 21));
	resume(create(sender, 2048, 10, "sender2-2", 2, p1, 22));
	resume(create(sender, 2048, 10, "sender2-3", 2, p1, 23));
	sleepms(500); //wait for senders to finish sending
	resume(p1);
	sleep(5);
	kill(p1);
	
	
	kprintf("\nTest 2: Registering a callback function\n");
	p1 = create(test_reg_cb,  1024, 10, "receiver_with_cb", 0);
	resume(p1);
	sleepms(500); //make sure receiver callback is registered
	resume(create(sender, 2048, 10, "sender3-1", 2, p1, 31));
	resume(create(sender, 2048, 10, "sender3-2", 2, p1, 32));
	resume(create(sender, 2048, 10, "sender3-3", 2, p1, 33));
	sleep(5);
	kill(p1);

	/* Sample test output:
	
	Test 0: Send (No blocking)
	FAILED Sending 12
	FAILED Sending 13
	Receiver Process 3, Message: 11

	Test 1: Send-blocking
	Receiver Process 7, Message: 21
	Receiver Process 7, Message: 22
	Receiver Process 7, Message: 23

	Test 2: Registering a callback function
	In callback of Process 11, Message: 31
	In callback of Process 11, Message: 32
	In callback of Process 11, Message: 33
	*/

	kprintf("\nCustom Tests\n ");
	
	kprintf("\nTest 3: Send-Blocking and Send\n");
	p1 = create(receiver, 1024, 10, "receiver", 0);
	resume(create(sender, 2048, 10, "sender4-1", 2, p1, 41));
	resume(create(sender_no_blk, 2048, 10, "sender4-2", 2, p1, 42));
	resume(create(sender, 2048, 10, "sender4-3", 2, p1, 43));
	resume(p1);
	sleep(5);
	kill(p1);

	kprintf("\nTest 4: Send-Blocking/norm-send with callbacks\n");
	p1 = create(test_reg_cb, 1024, 10, "receiver_cb", 0);
	resume(p1);
	sleep(500);
	resume(create(sender, 2048, 10, "sender5-1", 2, p1, 51));
	resume(create(sender_no_blk, 2048, 10, "sender5-2", 2, p1, 52));
	resume(create(sender, 2048, 10, "sender5-3", 2, p1, 53));
	sleep(5);
	kill(p1);
	
	kprintf("\nTest 5:Sending with multiple recievers\n");
	p1 = create(receiver, 1024, 10, "receiver1", 0);
	p2 = create(receiver, 1024, 10, "receiver2", 0);
	resume(create(sender, 2048, 10, "sender6-1", p1, 61));
	resume(create(sender, 2048, 10, "sender6-2", p2, 62));
	resume(create(sender, 2048, 10, "sender6-3", p1, 63));
	resume(create(sender, 2048, 10, "sender6-4", p2, 64));
	resume(create(sender, 2048, 10, "sender6-5", p1, 65));
	resume(create(sender, 2048, 10, "sender6-6", p2, 66));
	resume(p1);
	sleep(5);
	resume(p2);
	sleep(5);
	kill(p1);
	kill(p2);
	
	/*Not entirely sure what these custom tests should print
	but we'll figure it out*/







	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */
	
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}


