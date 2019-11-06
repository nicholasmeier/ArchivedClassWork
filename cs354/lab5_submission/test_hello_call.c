#define _GNU_SOURCE

#include <arch/x86/include/generated/uapi/asm/unistd_64.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(){
	printf("Calling...\n");
	syscall(__NR_my_hello_call);
	return 0;
}
