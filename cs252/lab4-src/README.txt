Part 1: 
	The output from step 3 is the mixed output from the various threads in the process. The process
	randomly spits out "A", "B", or "C" in varied segments as the different threads are iterated
	through.
	The program thr2 does not have 2 threads running because the main process calls printC("C") 
	before the threads are started. Thus the infinite loop in printC() will never return to the
	main method and never start the 2 threads. 


