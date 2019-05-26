#include <linux/module.h>
#include <linux/moduleparam.h>

int dumb_data;

module_param(dumb_data, int, 0);
MODULE_PARM_DESC(dumb_dato, "Dumb data for dumb driver");

int init_module(void)
{

	printk("INIT_MODULE: Exercise 01\n");
	printk("Driver data: %d\n", dumb_data);
	
	return 0;
}


void cleanup_module(void)
{
	printk("CLEANUP_MODULE: Exercise 01\n");
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("LINUX DRIVERS TRAINING: EXERCISE 01");

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(dumb_data);
