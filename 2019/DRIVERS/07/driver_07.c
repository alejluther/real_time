#include <linux/module.h>
#include <linux/major.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

#include "virt_dev_ioctl.h"

#define DEBUG

int virt_dev_major;
struct cdev *virt_dev_cdev;
static dev_t virt_dev_cdev_number;

static int
virt_dev_open(struct inode *inode, struct file *filp)
{
 
    printk("VIRT_DEV_OPEN\n");
	return 0;
}

static int
virt_dev_release(struct inode *inode, struct file *file)
{
	printk("VIRT_DEV_CLOSE\n");
	return 0;
}

static long
virt_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd){

		case VIRT_DEV_RESET:
			break;
		
		case VIRT_DEV_SET_ONLY_EVEN:
			break; 

		case VIRT_DEV_SET_UPPER_LIMIT:
			break; 

		default:
			printk("IOCTL: invalid command\n");
			return -1;
	}
	return 0;
}

static struct file_operations virt_dev_fops =
{
	read: 		virt_dev_read,
	write: 		virt_dev_write,
	open: 		virt_dev_open,
	poll: 		virt_dev_poll,
	unlocked_ioctl: 		virt_dev_ioctl,
	release: 	virt_dev_release,
};


int
init_module(void)
{
	int i, ret;
	printk("INIT_MODULE:Exercise 06\n");

	if (alloc_chrdev_region(&virt_dev_cdev_number, 0, 2, "virt_dev_simple") < 0){
		printk("virt_dev_simple: alloc_chrdev_region failed\n");
		return -1;
	};

	virt_dev_cdev = cdev_alloc();
	if (!virt_dev_cdev) {
		printk("cdev_alloc failed\n");
    	unregister_chrdev_region(virt_dev_cdev_number, 2);
		return -1;
	}

	cdev_init(virt_dev_cdev, &virt_dev_fops);
	ret = cdev_add(virt_dev_cdev, virt_dev_cdev_number, 2);
	if (ret) {
		printk("cdev_add failed\n");
	   	unregister_chrdev_region(virt_dev_cdev_number, 2);
	}

	return 0;
}


void
cleanup_module(void)
{
	printk("CLEANUP_MODULE: Exercise 07\n");

	unregister_chrdev_region(virt_dev_cdev_number, 2);
    cdev_del(virt_dev_cdev);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("CURSO LINUX TRAINING: Exercise 07");

MODULE_LICENSE("GPL");
