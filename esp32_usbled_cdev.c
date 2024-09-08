
#ifdef  MYDEV_CDEV

#define EP_MAX_BUF  64
#define MAX_PKT_SIZE  EP_MAX_BUF 

#define EP_IN   1
#define EP_OUT  2

extern struct usb_interface *my_interface;
extern struct usb_device *my_device;
extern struct usb_class_driver my_class;

extern uint8_t *datval ;

int opencount=0;

int psize = 4;


static int     _cdev_open(struct inode *, struct file * );
static ssize_t _cdev_read(struct file *filep,  char *buffer, size_t len, loff_t *offset);
static ssize_t _cdev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);
static int     _cdev_release(struct inode *, struct file * );


struct file_operations cdev_fops = {
        .owner   =       THIS_MODULE,
        .read    =       _cdev_read,
        .write   =       _cdev_write,
        .open    =       _cdev_open,
        .release =       _cdev_release,
};


static int _cdev_open(struct inode *inodep, struct file *filep){
   opencount++;


   printk(KERN_INFO "mydev_cdev: hardev open count: %d\n", opencount);
   return 0;
}


static ssize_t _cdev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int retval = 0;
   int rd_cnt = 0;
   int nc ;


  printk(KERN_INFO "in read len %d %d\n", (int)len, min( (int)len, MAX_PKT_SIZE));

   retval = usb_bulk_msg(my_device, usb_rcvbulkpipe(my_device, EP_IN), datval,
                   min( (int)len, MAX_PKT_SIZE), &rd_cnt, 5000);

   if (retval) printk(KERN_ERR "bulk read message returned %d\n", retval);
   if (retval) return retval;

    printk(KERN_INFO "Read %d bytes\n", rd_cnt );

   
    nc =  (int) min( (int)len, MAX_PKT_SIZE) ;
    if ( nc > psize ) nc  = psize;

    retval  = copy_to_user(buffer, datval, nc );
    if (retval) printk(KERN_ERR "error: bulk write message returned %d\n", retval);
    
    if (retval) return retval;

return rd_cnt ;
}



static ssize_t _cdev_write(struct file *filep,  const char *buffer, size_t len, loff_t *offset){
    int retval ; 
    int wr_cnt ;
    int nc ;

    printk("enter write\n");

    nc = (int) min( (int)len, MAX_PKT_SIZE);
    if ( nc > psize ) nc = psize ;
    retval = copy_from_user(datval, buffer,  nc );
    //printk(KERN_INFO "copy from user %02X %02X\n", buffer[0], buffer[1] );

    if (retval)
    {
        printk(KERN_INFO "copy from user failed\n");
        return retval;
    }


    retval = usb_bulk_msg(my_device, usb_sndbulkpipe(my_device, EP_OUT),
    		datval, (int) min( (int)len, MAX_PKT_SIZE), &wr_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk write message returned %d\n", retval );

        return retval;
    }
     printk(KERN_INFO "Written %d bytes\n", wr_cnt );


   return len;
}



static int _cdev_release(struct inode *inodep, struct file *filep){
   opencount--;


   printk(KERN_INFO "mydev_cdev: chardev closed\n");
   return 0;
}


static int __init _cdev_init(void){

    int retval;

    printk(KERN_INFO "mydev_cdev: Initializing cdev\n");

    my_class.name = "usb/mydev%d";
    my_class.fops = &cdev_fops;

    if ((retval = usb_register_dev(my_interface, &my_class) ) < 0)
    {
      printk(KERN_ALERT "myusb_cdev: could not register usb device \n");
      return retval ;
    }

    else printk(KERN_INFO "myusb_cdev: registered usb device /dev/mydev%d\n", 
		    my_interface->minor);

   return 0;

}



static void __exit _cdev_exit(void){


     printk(KERN_INFO "mydev_cdev: char device /dev/mydev%d unlinked\n", my_interface->minor);
     usb_deregister_dev(my_interface, &my_class);
}
#endif




