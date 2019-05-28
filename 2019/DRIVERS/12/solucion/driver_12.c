#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <asm/io.h>
#include <asm/delay.h>

#include "gpio_ioctl_rpi.h"

#define GPIO_BASEADDR	0x3f200000
#define GPIO_GPFSEL2_OFFSET	0x8
#define GPIO_GPSET0_OFFSET	0x1c
#define GPIO_GPCLR0_OFFSET	0x28
#define GPIO_GPLEV0_OFFSET	0x34

#define MAX_DATA_ARRAY  10

volatile void *gpio_regs;	

struct cdev *gpio_cdev;
static dev_t gpio_cdev_number;

static int
gpio_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int
gpio_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t
gpio_read(struct file *filp, char *buf, size_t count, loff_t* ppos)
{
	u32 reg_data;

	trace_printk("gpio_read\n");
	if (count == 0)
		return 0;

	if (count < sizeof(int))
		return -EINVAL;

	if (count > sizeof(int))
		count = sizeof(int);

	reg_data = readl(gpio_regs + GPIO_GPLEV0_OFFSET);
	reg_data &= (1 << 20);

	if (copy_to_user(buf, &reg_data, sizeof(int)))
		return -EFAULT;

	return count;
}

static void
gpio_trigger(void) {
	u32 reg_data;

	trace_printk("gpio_trigger_on\n");
	reg_data = (1 << 21);
	writel(reg_data, gpio_regs + GPIO_GPSET0_OFFSET);
	udelay(10);
	writel(reg_data, gpio_regs + GPIO_GPCLR0_OFFSET);
	trace_printk("gpio_trigger_off\n");
}

static long
gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd){

		case GPIO_TRIGGER:
			gpio_trigger();
			break; 

		default:
			printk("IOCTL: invalid command\n");
			return -1;
	}
	return 0;
}

static struct file_operations gpio_fops =
{
	read: 		gpio_read,
	open: 		gpio_open,
	unlocked_ioctl: 		gpio_ioctl,
	release: 	gpio_release,
};


int
init_module(void)
{
	int ret;
	u32 reg_data;
	volatile void *reg_offset;

	printk("INIT_MODULE: Exercise 12\n");

	/* Necesario para poder acceder al controlador 
	 * El kernel crea paginas logicas referenciando a las 
	 * * direcciones fisicas del controlador */

	gpio_regs = ioremap(GPIO_BASEADDR, 0xFFFF);
	printk("gpio_regs at %p\n", gpio_regs);

	if (alloc_chrdev_region(&gpio_cdev_number, 0, 1, "gpio_simple") < 0){
		printk("gpio_simple: alloc_chrdev_region failed\n");
		return -1;
	};

	gpio_cdev = cdev_alloc();
	if (!gpio_cdev) {
		printk("gpio_cdev_alloc failed\n");
    	unregister_chrdev_region(gpio_cdev_number, 1);
		return -1;
	}

	cdev_init(gpio_cdev, &gpio_fops);
	ret = cdev_add(gpio_cdev, gpio_cdev_number, 1);
	if (ret) {
		printk("cdev_add failed\n");
	   	unregister_chrdev_region(gpio_cdev_number, 1);
	}

	/* Set TRIGGER PIN as output */
	reg_offset = gpio_regs + GPIO_GPFSEL2_OFFSET;
	reg_data = readl(reg_offset);
	printk("GPIO_GPFSEL2(%p): %08x\n", reg_offset, reg_data);
	reg_data &= ~(0x38);
	reg_data |= (1 << 3);
	printk("Writing %08x to GPIO_GPFSEL2_OFFSET\n", reg_data);

	writel(reg_data, gpio_regs + GPIO_GPFSEL2_OFFSET);
	reg_data = readl(gpio_regs + GPIO_GPFSEL2_OFFSET);
	printk("GPIO_GPFSEL2: %08x\n", reg_data);

    return 0;
}


void
cleanup_module(void)
{
	unregister_chrdev_region(gpio_cdev_number, 1);
    cdev_del(gpio_cdev);
    iounmap((void *)gpio_regs);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("LINUX DRIVERS TRAINING: EJERCICIO 12");
MODULE_LICENSE("GPL");
