/*Luis Pozas Palomo 01946200-D, Ming yang Chen x9805942F*/

#include <linux/module.h>
#include <asm-generic/errno.h>
#include <linux/init.h>
#include <linux/tty.h>      /* For fg_console */
#include <linux/kd.h>       /* For KDSETLED */
#include <linux/vt_kern.h>
#include <linux/version.h> /* For LINUX_VERSION_CODE */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>	/* for copy_to_user */
#include <linux/cdev.h>
/*
 *  Prototypes
 */
int init_module(void);
void cleanup_module(void);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev_leds"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80		/* Max length of the message from the device */
#define ALL_LEDS_ON 0x7
#define ALL_LEDS_OFF 0

dev_t start;
struct cdev* chardev=NULL;

static struct file_operations fops = {
    .write = device_write
};


struct tty_driver* kbd_driver= NULL;


/* Get driver handler */
struct tty_driver* get_kbd_driver_handler(void)
{
    printk(KERN_INFO "chardev_leds: loading\n");
    printk(KERN_INFO "chardev_leds: fgconsole is %x\n", fg_console);
#if ( LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32) )
    return vc_cons[fg_console].d->port.tty->driver;
#else
    return vc_cons[fg_console].d->vc_tty->driver;
#endif
}

/* Set led state to that specified by mask */
static inline int set_leds(struct tty_driver* handler, unsigned int mask)
{
#if ( LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32) )
    return (handler->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,mask);
#else
    return (handler->ops->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, mask);
#endif
}


static int __init chardev_leds_init(void)
{
    kbd_driver= get_kbd_driver_handler();
    set_leds(kbd_driver,ALL_LEDS_ON);

    int major;		/* Major number assigned to our device driver */
    int minor;		/* Minor number assigned to the associated character device */
    int ret;

    /* Get available (major,minor) range */
    if ((ret=alloc_chrdev_region (&start, 0, 1,DEVICE_NAME))) {
        printk(KERN_INFO "Can't allocate chrdev_region()");
        return ret;
    }

    /* Create associated cdev */
    if ((chardev=cdev_alloc())==NULL) {
        printk(KERN_INFO "cdev_alloc() failed ");
        unregister_chrdev_region(start, 1);
        return -ENOMEM;
    }

    cdev_init(chardev,&fops);

    if ((ret=cdev_add(chardev,start,1))) {
        printk(KERN_INFO "cdev_add() failed ");
        kobject_put(&chardev->kobj);
        unregister_chrdev_region(start, 1);
        return ret;
    }

    major=MAJOR(start);
    minor=MINOR(start);

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'sudo mknod -m 666 /dev/%s c %d %d'.\n", DEVICE_NAME, major,minor);
    printk(KERN_INFO "Try to cat and echo to the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}

static void __exit chardev_leds_exit(void)
{
    set_leds(kbd_driver,ALL_LEDS_OFF);
    /* Destroy chardev */
    if (chardev)
        cdev_del(chardev);

    /*
     * Unregister the device
     */
    unregister_chrdev_region(start, 1);
}

module_init(chardev_leds_init);
module_exit(chardev_leds_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("chardev_leds");

/*
 * Called when a process writes to dev file: echo "hi" > /dev/chardev_leds
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	int i;
	unsigned int status = 0x0;
	set_leds(kbd_driver, ALL_LEDS_OFF);
	char kbuff[12]; //El comando no va a ocupar mas de 12
	copy_from_user(kbuff, buff, len);
	kbuff[len] = '\0';

	for (i = 0; i < len; ++i){
		switch (kbuff[i]){
			case '1': status = status | 0x2; break;
			case '2': status = status | 0x4; break;
			case '3': status = status | 0x1; break;
			case '\n': break;
			default: return -EINVAL;
		}
	}
	//printk("%u", status); //para ver en el log del sistema
	set_leds(kbd_driver, status);
	return len;
}

































