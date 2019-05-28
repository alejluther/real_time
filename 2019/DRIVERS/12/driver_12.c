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
#define GPIO_GPFSEL0_OFFSET	0x0
#define GPIO_GPFSEL1_OFFSET	0x4
#define GPIO_GPFSEL2_OFFSET	0x8
#define GPIO_GPSET0_OFFSET	0x1c
#define GPIO_GPCLR0_OFFSET	0x28
#define GPIO_GPLEV0_OFFSET	0x34

#define MAX_DATA_ARRAY  10

volatile void *gpio_regs;	

int
init_module(void)
{
	u32 reg_data;
	volatile void *reg_offset;

	printk("INIT_MODULE: Exercise 12\n");

	/* Necesario para poder acceder al controlador 
	 * El kernel crea paginas logicas referenciando a las 
	 * * direcciones fisicas del controlador */

	gpio_regs = ioremap(GPIO_BASEADDR, 0xFFFF);
	printk("gpio_regs at %p\n", gpio_regs);

	reg_offset = gpio_regs + GPIO_GPFSEL0_OFFSET;
	reg_data = readl(reg_offset);
	printk("GPIO_GPFSEL0(%p): %08x\n", reg_offset, reg_data);

	reg_offset = gpio_regs + GPIO_GPFSEL1_OFFSET;
	reg_data = readl(reg_offset);
	printk("GPIO_GPFSEL1(%p): %08x\n", reg_offset, reg_data);

	reg_offset = gpio_regs + GPIO_GPFSEL2_OFFSET;
	reg_data = readl(reg_offset);
	printk("GPIO_GPFSEL2(%p): %08x\n", reg_offset, reg_data);

    return 0;
}


void
cleanup_module(void)
{
    iounmap((void *)gpio_regs);
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("LINUX DRIVERS TRAINING: EJERCICIO 12");
MODULE_LICENSE("GPL");
