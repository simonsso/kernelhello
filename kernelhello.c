// simple example of a kernel hello world.
// As found on http://www.tldp.org/LDP/lkmpg/2.6/html/x121.html

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */

MODULE_LICENSE("GPL");

#define procfs_name "kernelhello"

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);
struct proc_dir_entry *Our_Proc_File;

int kernelhello; // Store a global in the kernel
int init_module(void)
{
	printk(KERN_INFO "Hello world 1.\n");

	Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);

	if (Our_Proc_File == NULL) {
		remove_proc_entry(procfs_name, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
				procfs_name);
		return -ENOMEM;
	}

	Our_Proc_File->read_proc = procfile_read;
	Our_Proc_File->mode 	 = S_IFREG | S_IRUGO;
	Our_Proc_File->uid 	 = 0;
	Our_Proc_File->gid 	 = 0;
	Our_Proc_File->size 	 = 18;


	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */

	return 0;
}

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data) {
	int ret;
	
	/* 
	 * We give all of our information in one go, so if the
	 * user asks us if we have more information the
	 * answer should always be no.
	 *
	 * This is important because the standard read
	 * function from the library would continue to issue
	 * the read system call until the kernel replies
	 * that it has no more information, or until its
	 * buffer is filled.
	 */
	if (offset > 0) {
		/* we have finished to read, return 0 */
		ret  = 0;
	} else {
		/* fill the buffer, return the buffer size */
		ret = sprintf(buffer, "Kernel %x helloworld!\n",kernelhello++);
	}

	return ret;
}

void cleanup_module(void)
{
	remove_proc_entry(procfs_name, NULL);
	printk(KERN_INFO "Goodbye world 1.\n");
}
