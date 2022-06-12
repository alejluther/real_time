#include <linux/module.h>
#include <linux/major.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

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

static ssize_t
virt_dev_write(struct file *filp, const char *buf, size_t count, loff_t* ppos)
{
	return 0;
}

static ssize_t
virt_dev_read(struct file *filp, char *buf, size_t count, loff_t* ppos)
{

	return 0;
}

static struct file_operations virt_dev_fops =
{
	read:			virt_dev_read,
	write:			virt_dev_write,
	open:			virt_dev_open,
	release:		virt_dev_release,
};


int
init_module(void)
{
	int i, ret;
	printk("INIT_MODULE: Exercise 04\n");

	ret = alloc_chrdev_region(&virt_dev_cdev_number, 0, 1, "virt_dev_simple");
	if (ret < 0) {
	    printk("virt_dev_simple: alloc_chrdev_region failed\n");
		return -1;
	};

	virt_dev_cdev = cdev_alloc();
	if (!virt_dev_cdev) {
		printk("cdev_alloc failed\n");
    	unregister_chrdev_region(virt_dev_cdev_number, 1);
		return -1;
	}

	cdev_init(virt_dev_cdev, &virt_dev_fops);
	ret = cdev_add(virt_dev_cdev, virt_dev_cdev_number, 1);
	if (ret) {
		printk("cdev_add failed\n");
	   	unregister_chrdev_region(virt_dev_cdev_number, 1);
    	cdev_del(virt_dev_cdev);
	}

    return 0;
}


void cleanup_module(void)
{
    printk("CLEANUP_MODULE: Ejercicio driver 04\n");

    unregister_chrdev_region(virt_dev_cdev_number, 1);
    cdev_del(virt_dev_cdev);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("LINUX DRIVERS TRAINING: Exercise 04");
MODULE_LICENSE("GPL");
