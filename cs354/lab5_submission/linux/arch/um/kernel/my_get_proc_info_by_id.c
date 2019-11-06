#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/uaccess.h>

asmlinkage long sys_my_get_proc_info_by_id(pid_t pid, void __user *user_buff){
	//get task struct using pid
	struct task_struct *t;
	t = find_task_by_vpid(pid);

	//if tas_struct is not null, get exe name of the task
	if (t == NULL){
		return -1;	
	}
	char buf[TASK_COMM_LEN];
	int priority = task_prio(t);
	get_task_comm(buf, t);
	
	//copy name of the task struct and the process prio to user buffer
	copy_to_user(user_buff, buf, sizeof(buf)); 
	copy_to_user(user_buff+16, &priority, 4);	

	return 0;
}
