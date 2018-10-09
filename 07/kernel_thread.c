#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kthread.h>  // for threads
#include <linux/sched.h>  // for task_struct
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>


static struct task_struct *thread1;

static int thread_fn(void *arg) {

	while (!kthread_should_stop()){
		printk("Real Time Thread ... waiting\n");
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(100); /* jiffies */
	}
	
	printk("Real Time Thread: Exiting\n");

	return 0;
}


int thread_init (void) {
	char our_thread[10]="KPERIODIC\0";
	printk(KERN_INFO "in init");
	thread1 = kthread_create(thread_fn,NULL,our_thread);
	if((thread1))
	{
		wake_up_process(thread1);
	
	}
	return 0;
}


void thread_cleanup(void) {
		   
	kthread_stop(thread1);

}
MODULE_LICENSE("GPL");   
module_init(thread_init);
module_exit(thread_cleanup);
