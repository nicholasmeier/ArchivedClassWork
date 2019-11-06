#define _GNU_SOURCE

#include <arch/x86/include/generated/uapi/asm/unistd_64.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

int main(int argc, char **argv) { 
	if(argc < 2){
		return 0;
	}
	int pid = atoi(argv[1]);
	int priority;
	char temp_buffer[21];
	printf("Calling ...\n");
	syscall(__NR_my_get_proc_info_by_id, pid, temp_buffer);
	memcpy(&priority, temp_buffer+16, 4);
	printf("name: %s %d\n", temp_buffer, priority);
}
