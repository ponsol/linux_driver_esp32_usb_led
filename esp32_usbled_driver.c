#include <linux/usb.h>


#include  "esp32_usbled_cdev.c"

#define MYUSB_VENDOR_ID   (0x303a)
#define MYUSB_PRODUCT_ID  (0x4008)



static struct usb_device_id myusb_table [] = {
        { USB_DEVICE(MYUSB_VENDOR_ID, MYUSB_PRODUCT_ID) },
        { }
};


MODULE_DEVICE_TABLE (usb, myusb_table);

struct usb_device *my_device;
struct usb_class_driver my_class;
struct usb_interface *my_interface;



uint8_t *datval ;

static int myusb_probe(struct usb_interface *interface, const struct usb_device_id *id) {


    struct usb_host_interface *iface_desc;
    //struct usb_endpoint_descriptor *endpoint;

    int retval ;


    my_device = interface_to_usbdev(interface);

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "device %d probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber,
            id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);


#ifdef MYDEV_CDEV
    my_interface = interface ;
    retval = _cdev_init();
    if ( retval < 0 ) {
      printk(KERN_INFO "myusb_driver: cdev error\n");
      return retval ;
    }
#endif

    return 0;
}

static inline void myusb_disconnect(struct usb_interface *interface) {


#ifdef MYDEV_CDEV
    _cdev_exit();
#endif


   return;
}



static struct usb_driver myusb_driver = {
        .name        = "My_ESP32_USB",
        .probe       = myusb_probe,
        .disconnect  = myusb_disconnect,
        .id_table    = myusb_table,
        //.supports_autosuspend = 1,
};


static int myusb_driver_init(void){
   int result;
   result = usb_register(&myusb_driver);

   if (result < 0) {
       printk(KERN_ERR "myusb_driver: usb_register failed for the %s driver. Error number %d\n",
       myusb_driver.name, result);
   } else {
     printk(KERN_INFO "myusb_driver: usb_register success\n");
   }

    if ( datval == NULL ) datval = kmalloc(EP_MAX_BUF, GFP_KERNEL);

   return result ;

}

void  myusb_driver_exit(void){


   if (datval !=NULL ) kfree(datval);

   usb_deregister(&myusb_driver);

   printk(KERN_INFO "myusb_driver: deregister success for device %s\n", myusb_driver.name);
}

