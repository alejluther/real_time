#include <linux/module.h>
#include <linux/major.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

#define DEBUG

int virt_dev_major;
struct cdev *virt_dev_cdev;
static dev_t virt_dev_cdev_number;

int deposit_value = 10000000;

static int
virt_dev_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int
virt_dev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t
virt_dev_write(struct file *filp, const char *buf, size_t count, loff_t* ppos)
{
	deposit_value += (int)count;
	return count;
}

static ssize_t
virt_dev_read(struct file *filp, char *buf, size_t count, loff_t* ppos)
{

	deposit_value -= count;
	return count;
}

static struct file_operations virt_dev_fops =
{
	read: 		virt_dev_read,
	write: 		virt_dev_write,
	open: 		virt_dev_open,
	release: 	virt_dev_release,
};


int
init_module(void)
{
	int ret;

	printk("INIT_MODULE:Exercise 17\n");

	if (alloc_chrdev_region(&virt_dev_cdev_number, 0, 1, "virt_dev_bank") < 0){
		printk("virt_dev_bank: alloc_chrdev_region failed\n");
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
	printk("CLEANUP_MODULE: Exercise 17\n");

	unregister_chrdev_region(virt_dev_cdev_number, 1);
    cdev_del(virt_dev_cdev);
	printk("Deposit value: %d\n", deposit_value);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("CURSO LINUX TRAINING: Exercise 17");

MODULE_LICENSE("GPL");
