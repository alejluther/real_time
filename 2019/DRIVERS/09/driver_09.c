#include <linux/module.h>
#include <linux/major.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

#include <linux/slab.h>

#include "virt_dev_ioctl.h"

#define DEBUG

#define MAX_MINOR_DEVICES 2
#define MINOR_0 0
#define MINOR_1 1

#define MAX_DATA_ARRAY  10

#define IS_IMPAR(x) (x % 2)
#define IS_INVALID(x) (x > ptr->flag_upper_limit)

int virt_dev_major;
struct cdev *virt_dev_cdev;
static dev_t virt_dev_cdev_number;

struct data_elem{
    int value;
    struct data_elem *next;
};

struct numbers_data{
	wait_queue_head_t read_queue;
	struct data_elem *head, *tail;
	int total_count;
	int flag_only_par;
	int flag_upper_limit;
	struct kmem_cache *slab_cache;
};

struct numbers_data s_num[MAX_MINOR_DEVICES];

static int
virt_dev_open(struct inode *inode, struct file *filp)
{
 
#ifdef DEBUG
    printk("VIRT_DEV_OPEN\n");
#endif
	/* sanity check */
	if (MINOR(inode->i_rdev) >= MAX_MINOR_DEVICES) {
		printk("Este driver solo soporta %d dispositos\n", MAX_MINOR_DEVICES);
		return -ENODEV;
	}

#ifdef DEBUG
    printk("DRIVER: opening file minor %u\n", MINOR(inode->i_rdev));
#endif

	/* Linking file object with the internal data struct per minor device */
	filp->private_data = (struct numbers_data *)&s_num[MINOR(inode->i_rdev)];

	return 0;
}

static int
virt_dev_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk("VIRT_DEV_CLOSE\n");
#endif

	return 0;
}

static int
elem_insert(struct numbers_data *ptr, int data)
{

	struct data_elem *new;

	new = kmem_cache_alloc(ptr->slab_cache, GFP_KERNEL);
	if (new == NULL) {
		printk("elem_insert: kmalloc error\n");
		return -1;
	}

	new->value = data;
	new->next = NULL;

	if (ptr->head == NULL) {
		ptr->head = new;
		ptr->tail = new;
	} else {
		ptr->tail->next = new;
		ptr->tail = new;
	}

	ptr->total_count++;

#ifdef DEBUG
    printk("elem_insert: total_count = %d\n", ptr->total_count);
#endif
    return 0;
}

static ssize_t
virt_dev_write(struct file *filp, const char *buf, size_t count, loff_t* ppos)
{
	int new_data;
	struct numbers_data *ptr = (struct numbers_data *)filp->private_data;

#ifdef DEBUG
	printk("virt_dev_write: count %u (%d)\n", count, ptr->total_count);
#endif

	if (count == 0)
		return 0;

	if (count < sizeof(int))
		return -EFAULT;

	if (count > sizeof(int))
		count = sizeof(int);

	if (ptr->total_count == MAX_DATA_ARRAY)
		return -ENOSPC;

	if (copy_from_user(&new_data, buf, sizeof(int))) {
		printk("write operation with bad data\n");
		return -EFAULT;
	}

#ifdef DEBUG
	printk("FLAG_ONLY_PAR: %d, FLAG_UPPER_LIMIT: %d\n", ptr->flag_only_par,
			ptr->flag_upper_limit);
#endif
	if (ptr->flag_only_par && IS_IMPAR(new_data))
		return -ENXIO;

	if (ptr->flag_upper_limit && IS_INVALID(new_data))
		return -ENXIO;

	if (elem_insert(ptr, new_data) < 0)
		return -EFAULT;

	wake_up(&ptr->read_queue); 

#ifdef DEBUG
	printk("virt_dev_write: exiting (%d)\n", ptr->total_count);
#endif

	return sizeof(int);
}

static ssize_t
virt_dev_read(struct file *filp, char *buf, size_t count, loff_t* ppos)
{

	struct numbers_data *ptr = (struct numbers_data *)filp->private_data;
	struct data_elem *head;
	int data;

#ifdef DEBUG
	printk("virt_dev_read: count %u (%d)\n", count, ptr->total_count);
#endif

	if (count == 0)
		return 0;

	if (count < sizeof(int))
		return -EFAULT;

	if (count > sizeof(int))
		count = sizeof(int);

	if (ptr->total_count == 0 && filp->f_flags & O_NONBLOCK)
		return 0;

	wait_event_interruptible(ptr->read_queue, ptr->total_count != 0);

	if (ptr->total_count == 0)
		return 0;

	data = ptr->head->value;
	if (copy_to_user(buf, &data, sizeof(int)))
		return -EFAULT;

	head = ptr->head;
	ptr->head = head->next;
	ptr->total_count--;
	kmem_cache_free(ptr->slab_cache, head);
    
	return count;
}

static long
virt_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct numbers_data *ptr = (struct numbers_data *)filp->private_data;
	struct data_elem *elem, *aux;

	switch (cmd){

		case VIRT_DEV_RESET:
			printk("IOCTL: RESET\n");
			ptr->total_count = 0;
			elem = ptr->head;
			while (elem) {
				aux = elem;
				elem = elem->next;
				kfree(aux);
			}
			break;
		
		case VIRT_DEV_SET_ONLY_EVEN:
			printk("IOCL: SET_ONLY_EVEN\n");
			ptr->flag_only_par = 1;
			break; 

		case VIRT_DEV_SET_UPPER_LIMIT:
			printk("IOCL: SET_UPPER_LIMIT\n");
			if (copy_from_user(&ptr->flag_upper_limit, (int *)arg, sizeof(int)))
				return -EFAULT;

			printk("Setting upper limit to %d\n", ptr->flag_upper_limit);
			break; 

		default:
			printk("IOCTL: invalid command\n");
			return -1;
	}
	return 0;
}

unsigned int
virt_dev_poll (struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	unsigned int minor = iminor(filp->f_path.dentry->d_inode);
	struct numbers_data *ptr = (struct numbers_data *)filp->private_data;

#ifdef DEBUG
	printk("virt_dev_poll for minor %u\n", minor);
#endif

	poll_wait(filp, &ptr->read_queue, wait);

	if (ptr->total_count)
		mask |= POLLIN;

	return mask;
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

	/* Reseteando valores de las estructuras de datos */
	for (i = 0; i < MAX_MINOR_DEVICES; i++) {
		s_num[i].total_count = 0;
		init_waitqueue_head(&s_num[i].read_queue);
		s_num[i].flag_only_par = 0;
		s_num[i].flag_upper_limit = 0;
		s_num[i].head = NULL;
		s_num[i].tail = NULL;
		s_num[i].slab_cache = kmem_cache_create("virt_dev_slab",
								sizeof(struct data_elem), 0, 0, NULL);

		if (!s_num[i].slab_cache) {
			int j;
			printk("slab cache alloc failed for %i device\n", i);

			for (j = 0; j < i; j++)
    			kmem_cache_destroy(s_num[j].slab_cache);

			return -ENOMEM;
		}
	}

	if (alloc_chrdev_region(&virt_dev_cdev_number, 0, 2, "virt_dev_simple") < 0){
		printk("virt_dev_simple: alloc_chrdev_region failed\n");
		goto failed;
	};

	virt_dev_cdev = cdev_alloc();
	if (!virt_dev_cdev) {
		printk("cdev_alloc failed\n");
    	unregister_chrdev_region(virt_dev_cdev_number, 2);
		goto failed_cdev;
	}

	cdev_init(virt_dev_cdev, &virt_dev_fops);
	ret = cdev_add(virt_dev_cdev, virt_dev_cdev_number, 2);
	if (ret) {
		printk("cdev_add failed\n");
		goto failed_cdev_init;
	}

	return 0;

failed_cdev_init:
    cdev_del(virt_dev_cdev);
failed_cdev:
	unregister_chrdev_region(virt_dev_cdev_number, 2);
failed:

	for (i = 0; i < MAX_MINOR_DEVICES; i++)
    			kmem_cache_destroy(s_num[i].slab_cache);

	return -1;
}


void
cleanup_module(void)
{
	struct data_elem *aux;
	int i;

	printk("CLEANUP_MODULE: Exercise 09\n");

	unregister_chrdev_region(virt_dev_cdev_number, 2);
    cdev_del(virt_dev_cdev);

	for (i = 0; i < MAX_MINOR_DEVICES; i++) {
		while (s_num[i].head) {
			aux = s_num[i].head;
			s_num[i].head = s_num[i].head->next;
			kmem_cache_free(s_num[i].slab_cache, (void *)aux);
		}
	}
}

MODULE_AUTHOR("Alejandro Lucero <alucero@os3sl.com>");
MODULE_DESCRIPTION("CURSO LINUX TRAINING: Exercise 09");

MODULE_LICENSE("GPL");
