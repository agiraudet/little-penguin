#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>
MODULE_LICENSE("Dual BSD/GPL");

static const struct usb_device_id id_table[] = {
	    { .match_flags = USB_DEVICE_ID_MATCH_INT_CLASS,
			        .bInterfaceClass = USB_INTERFACE_CLASS_HID },
		    {  }
};

MODULE_DEVICE_TABLE(usb, id_table);

static int hello_init(void)
{
	printk(KERN_ALERT "[kbd] Hello world !\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "[kbd] Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
