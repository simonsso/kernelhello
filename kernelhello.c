// simple example of a kernel hello world.
// As found on http://www.tldp.org/LDP/lkmpg/2.6/html/x121.html

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");


char my_data[1024]="example"; /* our device */

int my_open(struct inode *inode,struct file *filep);
int my_release(struct inode *inode,struct file *filep);
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp );
   
struct file_operations my_fops={
   open: my_open,
   read: my_read,
   write: my_write,
   release:my_release,
};

int my_open(struct inode *inode,struct file *filep)
{
  // ???  MOD_INC_USE_COUNT; /* increments usage count of module */
   return 0;
}

int my_release(struct inode *inode,struct file *filep)
{
   // ?? MOD_DEC_USE_COUNT; /* decrements usage count of module */
   return 0;
}
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp )
{
   int mysize;
   mysize=strlen(my_data);
   mysize=(mysize>80)?80:mysize;
   /* function to copy kernel space buffer to user space*/
   if ( copy_to_user(buff,my_data,mysize) != 0 ){
      printk( "Kernel -> userspace copy failed!\n" );
   }
   return mysize;

}
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp )
{
   /* function to copy user space buffer to kernel space*/
   int mysize;
   int i;
   volatile int *GPSET0;
   volatile int *GPCLR0;
   GPSET0=(int*)0xf220001c;
   GPCLR0=(int*)0xf2200028;
  
   mysize=(count>80)?80:count;
   printk("my_write %d \n",(int) count);
   if ( copy_from_user(my_data,buff,mysize) != 0 ){
      printk( "Userspace -> kernel copy failed!\n" );
   }else{
      
      my_data[79]=0;
      my_data[mysize]=0;
      printk(my_data);   //store some debuging logs
      for(i=0;i<12;i++){
          if(my_data[i]=='0'){
             *GPSET0=0x00004000;
             udelay(350);
             *GPCLR0=0x00004000;
             udelay(1050);
             *GPSET0=0x00004000;
             udelay(350);
             *GPCLR0=0x00004000;
             udelay(1050);
          }else if(my_data[i]=='X'){
             *GPSET0=0x00004000;
             udelay(350);
             *GPCLR0=0x00004000;
             udelay(1050);
             *GPSET0=0x00004000;
             udelay(1050);
             *GPCLR0=0x00004000;
             udelay(350);
          }
      }
      *GPSET0=0x00004000;
      udelay(350);
      *GPCLR0=0x00004000;

   }
   return mysize;
}


int kernelhello; // Store a global in the kernel


int init_module(void)
{
   int *p;
	printk(KERN_INFO "Hello world 1.\n");
   if(register_chrdev(222,"my_device",&my_fops)){
      printk("failed to register");
   }
   
   // All gpio are inputs execpt gpio14
   // FUNSEL0-3 all 0 expept for but 14-12 in FUNSEL1 which are "001"
   p=(int *)0xf2200004;
   *p=0x00001000;

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */

	return 0;
}

void cleanup_module(void)
{
   unregister_chrdev(222,"my_device");
	printk(KERN_INFO "Goodbye world 1.\n");
}
