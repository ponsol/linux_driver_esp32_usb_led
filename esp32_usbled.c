

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("sjk");
MODULE_DESCRIPTION("My USB device");
MODULE_VERSION("0.1");


#include  "esp32_usbled_driver.c"


static int __init mod_init(void){

   printk(KERN_INFO "myusb: Loading myusb module\n");
   return myusb_driver_init();

  return 0;
}

static void __exit mod_exit(void){

   printk(KERN_INFO "myusb: Unloading myusb module\n");
   myusb_driver_exit();
}

module_init(mod_init);
module_exit(mod_exit);

