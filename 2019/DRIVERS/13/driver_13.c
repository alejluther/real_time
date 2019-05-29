#include <linux/module.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/mm.h>

#define GPIO_BASEADDR	0x3f200000
#define GPIO_GPFSEL0_OFFSET	0x0
#define GPIO_GPFSEL1_OFFSET	0x4
#define GPIO_GPFSEL2_OFFSET	0x8
#define GPIO_GPSET0_OFFSET	0x1c
#define GPIO_GPCLR0_OFFSET	0x28
#define GPIO_GPLEV0_OFFSET	0x34

volatile void *gpio_regs;	

struct cdev *gpio_cdev;
static dev_t gpio_cdev_number;

int
gpio_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret; 
    
	printk("gpio_mmap\n");
	ret = io_remap_pfn_range(vma, vma->vm_start, GPIO_BASEADDR >> PAGE_SHIFT,
							vma->vm_end - vma->vm_start, vma->vm_page_prot); 

	if (ret < 0)
		return -EAGAIN;

	return 0;
}


static struct file_operations gpio_fops =
{
	mmap: 		gpio_mmap,
};

int
init_module(void)
{
	int ret;

	printk("INIT_MODULE: Exercise 13\n");

	gpio_regs = ioremap(GPIO_BASEADDR, 0xFFFF);

	if (alloc_chrdev_region(&gpio_cdev_number, 0, 1, "gpio_mmap") < 0){
		printk("gpio_mmap: alloc_chrdev_region failed\n");
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

    return 0;
}


void
cleanup_module(void)
{
	unregister_chrdev_region(gpio_cdev_number, 1);
    cdev_del(gpio_cdev);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("CURSO LINUX AVANZADO: EJERCICIO 12");
MODULE_LICENSE("GPL");
