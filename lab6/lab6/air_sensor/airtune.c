/*****************************************************************************

DESCRIPTION (optional):


--------------------------------------------------------------------------


*****************************************************************************/


/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "AirFlowSensor.h"


/*--------------------  C o n s t a n t s  ---------------------------------*/

#define DEV_MAJOR 0		//Means to use a dynamic 
#define DEV_NAME "airtune"


#define SUCCESS 0


/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

static int major_number;

/*--------------------  F u n c t i o n s  ---------------------------------*/

// open function - called when the "file" /dev/ is opened in userspace
static int dev_open (struct inode *inode, struct file *file) 
{
	printk("%s:  device driver open\n", DEV_NAME);
	return 0;
}


// close function - called when the "file" /dev/airtune is closed in userspace  
static int dev_release (struct inode *inode, struct file *file)
{
	printk("%s:  device driver closed\n", DEV_NAME);
	return 0;
}

static ssize_t dev_read (struct file *file, char *buf, size_t count, loff_t *ppos) 
{
	printk("%s:  device driver read\n", DEV_NAME);
	return 0;
}

static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *ppos) 
{
	printk("%s:  device driver write\n", DEV_NAME);
	return 0;
}

// define which file operations are supported
struct file_operations dev_fops = 
{
	.owner	=	THIS_MODULE,
	.llseek	=	NULL,
	.read		=	dev_read,
	.write	=	dev_write,
	.readdir	=	NULL,
	.poll		=	NULL,
	.ioctl	=	NULL,
	.mmap		=	NULL,
	.open		=	dev_open,
	.flush	=	NULL,
	.release	=	dev_release,
	.fsync	=	NULL,
	.fasync	=	NULL,
	.lock		=	NULL,
	//	.readv	=	NULL,
	//	.writev	=	NULL,
};



// initialize module 
static int __init dev_init_module (void) 
{
	major_number = register_chrdev( 0, DEV_NAME, &dev_fops);

	if (major_number < 0) 
	{
	  printk(KERN_ALERT "Registering char device %s failed with %d\n", DEV_NAME, major_number);
	  return major_number;
	}		
	
	printk(KERN_INFO "%s was assigned major number %d. To talk to\n", DEV_NAME, major_number);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEV_NAME, major_number);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
	

	return SUCCESS;
}

// close and cleanup module
static void __exit dev_cleanup_module (void) 
{
	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(major_number, DEV_NAME);

		
	printk("%s: dev_cleanup_module Device Driver Removed\n", DEV_NAME);
}

module_init(dev_init_module);
module_exit(dev_cleanup_module);
MODULE_AUTHOR("Morten Mossige, University of Stavanger");
MODULE_DESCRIPTION("Linux devicedriver for tuning air sensors");
MODULE_LICENSE("GPL");
