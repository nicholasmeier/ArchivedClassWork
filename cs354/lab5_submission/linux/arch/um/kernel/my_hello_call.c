#include <linux/kernel.h>

asmlinkage long sys_my_hello_call(void){
	printk(KERN_ALERT "Hello world !!!\n"); //KERN_ALERT = loglevel, msg appears on console
	return 0;
}
