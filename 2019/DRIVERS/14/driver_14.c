#include <linux/module.h>
#include <asm/io.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#include "gpio_ioctl_rpi.h"

struct cdev *ultrasound_cdev;
static dev_t ultrasound_cdev_number;
struct class *ultrasound_class;
int total_minors = 1;
int echo_irq;

static irqreturn_t echo_irq_handler(int irq, void *dev_id);

#define MAX_SAMPLES 600
uint32_t samples[MAX_SAMPLES];
int wr_inx = 0;
int r_inx = 0;

static irqreturn_t
echo_irq_handler(int irq, void *dev_id)
{
	struct timeval t1;

	do_gettimeofday(&t1);
	trace_printk("ULTRASOUND ECHO INTERRUP: %ld-%ld T!!!\n", t1.tv_sec, t1.tv_usec);
	return IRQ_HANDLED;
}


static void 
gpio_shoot(void)
{

	/* Setting trigger signal  */
	gpio_set_value(21, 1);

	trace_printk("Trigger signal for pin 21\n");

	/* Trigger needs to be at least 10us */
	udelay(10);
	gpio_set_value(21, 0);
}

long
ultrasound_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd){

		case GPIO_TRIGGER:
			gpio_shoot();
			return 0;

		default:
			return -1;
	}
	return 0;
}

static ssize_t
ultrasound_read(struct file *filp, char *buf, size_t count, loff_t* ppos)
{
	int res;
	
	if (wr_inx == r_inx)
		return 0;

	res = copy_to_user(buf, &samples[r_inx++], 4);
	if (r_inx == MAX_SAMPLES)
		r_inx = 0;
	
	return 4;
}


static int
ultrasound_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int
ultrasound_release(struct inode *inode, struct file *file)
{
    return 0;
}

static struct file_operations ultrasound_fops =
{
	open: ultrasound_open,
	read: ultrasound_read,
	unlocked_ioctl: ultrasound_ioctl,
	release: ultrasound_release,
};

static void gpio_release(void) {
    
	gpio_free(20);
	gpio_free(21);

}

static int gpio_init(void)
{
   	int res;
	unsigned long irq_flags;

	res = gpio_request(20, "GPIO201_ECHO");
	if (res < 0) {
		printk("Ultrasound: GPIO1_20 request failed\n");
		return -1;
	}

	gpio_direction_output(20, 1);
	
	res = gpio_request(21, "GPIO21_TRIGGER");
	if (res < 0) {
		printk("Ultrasound: GPIO21 request failed\n");
		gpio_free(20);
		return -1;
	}

	gpio_direction_output(21, 0);
	
	echo_irq = gpio_to_irq(20);
	if (echo_irq < 0) {
		printk("Interrupt for GPIO echo not available\n");
		gpio_release();
		return -1;
	}

	irq_flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
	
	if ((request_irq (echo_irq, echo_irq_handler, irq_flags,
						"HB_ULTRASOUND_ECHO", 0)) == 0) {
		printk("Hackabone Ultrasound: request_irq gets IRQ %d\n", echo_irq);
	} else {
		printk("Hackabone Ultrasound: request_irq for irq %d failed\n", echo_irq);
		gpio_release();
		return -1;
	}
	return 0;
}


int init_module(void)
{
	printk("INIT_MODULE: RPI UltraSound\n");

	if (gpio_init() < 0)
		return -1;

	if (alloc_chrdev_region(&ultrasound_cdev_number, 0, 1,
							"rpi_ultrasound") < 0) {
		printk("rpi_ultrasond: can not allocate region\n");
		goto cleanup;
	};

	ultrasound_cdev = cdev_alloc();
	cdev_init(ultrasound_cdev, &ultrasound_fops);
	cdev_add(ultrasound_cdev, ultrasound_cdev_number, 1);

	ultrasound_class = class_create(THIS_MODULE, "rpi_ultra");

	device_create(ultrasound_class, NULL, MKDEV(MAJOR(ultrasound_cdev_number),
				MINOR(ultrasound_cdev_number)), NULL, "rpi_ultra_0");

	return 0;

cleanup:

	gpio_release();
	return -1;
}


void cleanup_module(void)
{
	free_irq(echo_irq, NULL);
	
	device_destroy(ultrasound_class,
				   MKDEV(MAJOR(ultrasound_cdev_number),
				   MINOR(ultrasound_cdev_number)));

	class_destroy(ultrasound_class);
	unregister_chrdev_region(ultrasound_cdev_number, 1);
	cdev_del(ultrasound_cdev);

	gpio_release();
	printk("CLEANUP_MODULE: UltraSound\n");
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("Ultrasound Driver");
MODULE_LICENSE("GPL");

