/*****************************************************************************

DESCRIPTION:  A very simple LKM

*****************************************************************************/

/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */

/*--------------------  C o n s t a n t s  ---------------------------------*/

#define DEV_NAME "simplkm"

/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

/*--------------------  F u n c t i o n s  ---------------------------------*/

int init_module(void)
{
	printk(KERN_INFO "Hello world! I'm %s and I'm being installed!\n",DEV_NAME);
	/*
	* A non 0 return means init_module failed; module can't be loaded.
	*/
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world, the %s lko is being removed.\n", DEV_NAME);
}

MODULE_AUTHOR("Morten Mossige, University of Stavanger");
MODULE_DESCRIPTION("Simple Linux devicedriver");
MODULE_LICENSE("GPL");
