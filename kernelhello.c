// simple example of a kernel hello world.
// As found on http://www.tldp.org/LDP/lkmpg/2.6/html/x121.html

/* Needed by all modules */
#include <linux/module.h>

/* Needed for KERN_INFO */
#include <linux/kernel.h>

/* Necessary because we use the proc fs */
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");


int kernelhello; // Store a global in the kernel
int init_module(void)
{
	printk(KERN_INFO "Hello world 1.\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
