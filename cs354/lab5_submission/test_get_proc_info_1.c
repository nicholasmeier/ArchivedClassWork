#define _GNU_SOURCE

#include <arch/x86/include/generated/uapi/asm/unistd_64.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

/* test_get_proc_info_1 - custom test 1 */

int main() {
	int i = 0;
        for(; i < 400; i++){
		int priority;
	        char temp_buffer[21];
        	printf("Calling with pid = %d\n", i);
	        if (syscall(__NR_my_get_proc_info_by_id, i, temp_buffer) == -1){
			printf("invalid pid\n");	
		}else{
	        	memcpy(&priority, temp_buffer+16, 4);
		        printf("name: %s %d\n", temp_buffer, priority);
		}
	}
	return 0;
}
