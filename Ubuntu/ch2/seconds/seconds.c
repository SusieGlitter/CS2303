#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"

ssize_t proc_read(struct file *file,char *buf,size_t count,loff_t *pos);

static struct file_operations proc_ops={
	.owner=THIS_MODULE,
	.read=proc_read,
};

int proc_init(void)
{
	proc_create(PROC_NAME,0,NULL,&proc_ops);
	printk(KERN_INFO "/proc/%s created\n",PROC_NAME);
	return 0;
}

void proc_exit(void)
{
	remove_proc_entry(PROC_NAME,NULL);
	printk(KERN_INFO "/proc/%s removed\n",PROC_NAME);
}

ssize_t proc_read(struct file *file,char __user *usr_buf,size_t count,loff_t *pos)
{
	int rv=0;
	char buffer[BUFFER_SIZE];
	static int completed=0;
	if(completed)
	{
		completed=0;
		return 0;
	}
	completed=1;

	rv=sprintf(buffer,"seconds:%lu\n",jiffies/HZ);
	copy_to_user(usr_buf,buffer,rv);
	return rv;
}


module_init(proc_init);
module_exit(proc_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("SGG");














