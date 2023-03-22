#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/wait.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/io.h>
#include <asm/page.h>

#include <linux/device.h>
#include <asm/irq.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irqflags.h>
#include <linux/wait.h>

#define DRIVER_NAME "bh"
#define DEVICE_NAME "black_hole"
#define BUFFER_SIZE     512
#define DEVICE_MAJOR    254
#define DEVICE_MINOR    0
#define DEVICE_BASE     0x20000000

MODULE_LICENSE("GPL");

static int cal_major = DEVICE_MAJOR;
static int cal_minor = DEVICE_MINOR;

static char buffer[BUFFER_SIZE];
void __iomem* base;

static int cal_open(struct inode* inode, struct file* file)
{
    return 0;
}

static int cal_release(struct inode* inode, struct file* file)
{
    return 0;
}

static ssize_t cal_read(struct file* filp, char* buff, size_t len, loff_t* off)
{
    int i;

    i = copy_to_user(buff, buffer, len);

    return len;
}

static ssize_t cal_write(struct file* filp, const char* buff, size_t len, loff_t* off)
{
    int i;

    ssize_t failed_len;

    if(len>BUFFER_SIZE)
    {
        len=BUFFER_SIZE;
    }

    //printk("write length: %d, c = %d\n", len, c);

    failed_len = copy_from_user(buffer, buff, len);

    len -= failed_len;

    for(i = 0; i < len; i++)
    {
        writeb(buffer[i], base);
    }
    /*
    for(i = 0; i < len/4; i++)
    {
        writel( *(((int*)buffer)+i), base);
        if( (buffer[i]!=0) && (buffer[i]!=0xFF) ) {
            printk("0x%X\n", buffer[i]);
        }
    }
    for(i=len%4;i>0;i--)
    {
        writeb(buffer[len-1-i], base);
    }
    */

    return len;
}

static long cal_unlocked_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static void allocate_memory_region(void)
{
    base = ioremap_nocache(DEVICE_BASE , 0x1000);
    printk("black hole physical address: %X\n", DEVICE_BASE);
    //printk("After ioremap: %08X\n", (int)base);
}

static void free_allocated_memory(void)
{
    iounmap((unsigned long*) base);
}

static struct file_operations cal_fops =
{
    .owner              = THIS_MODULE,
    .read               = cal_read,
    .write          = cal_write,
    .unlocked_ioctl     = cal_unlocked_ioctl,
    .open               = cal_open,
    .release            = cal_release
};

static struct cdev cal_cdev =
{
    .kobj       =   { .name = DRIVER_NAME },
    .owner      =   THIS_MODULE,
};

static int __init cal_init(void)
{
    dev_t dev = 0;
    int result;

    if(DEVICE_MAJOR)
    {
        dev = MKDEV(cal_major, cal_minor);
        result = register_chrdev_region(dev, 1, DRIVER_NAME);
    }
    else
    {
        result = alloc_chrdev_region(&dev, cal_minor, 1, DRIVER_NAME);
        cal_major = MAJOR(dev);
    }
    if(result < 0)
    {
        printk(KERN_ERR DRIVER_NAME "Can't allocate major number %d for %s\n", cal_major, DEVICE_NAME);
        return -EAGAIN;
    }
    cdev_init(&cal_cdev, &cal_fops);
    result = cdev_add(&cal_cdev, dev, 1);
    if(result < 0)
    {
        printk(KERN_ERR DRIVER_NAME "Can't add device %d\n", dev);
        return -EAGAIN;
    }

    allocate_memory_region();

    return 0;
}

static void __exit cal_exit(void)
{
    dev_t dev;
    free_allocated_memory();
    dev = MKDEV(cal_major, cal_minor);
    unregister_chrdev_region(dev, 1);
    cdev_del(&cal_cdev);
    //printk("calculator driver exit\n");
}

module_init(cal_init);
module_exit(cal_exit);

