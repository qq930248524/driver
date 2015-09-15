#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

#include "hello.h"

MODULE_LICENSE("Dual BSD/GPL");


int hello_major = HELLO_MAJOR;
int hello_minor = 0;
int hello_nr_devs = HELLO_NR_DEVS;
char deviceName[10] = {0};

module_param(hello_major, int, S_IRUGO);
module_param(hello_minor, int, S_IRUGO);
module_param(deviceName, charp, S_IRUGO);

dev_t devId;

static int __init hello_init(void)
{
    dev_t dev;

    if(hello_major){
        dev = MKDEV(hello_major, hello_minor);
        if(register_chrdev_region(dev, hello_nr_devs, deviceName) != 0)   goto fail;
    }else{
        if(alloc_chrdev_region(&dev, 0, hello_nr_devs, deviceName) != 0)  goto fail;
        hello_major = MAJOR(dev);
    }

    if(alloc_chrdev_region(&devId, 0, 2, "zjy_") != 0)
        goto fail;
    return 0;

fail:
    unregister_chrdev_region(devId, 2);
}

static int __exit hello_exit(void)
{
    printk(KERN_ALERT "exit\n");
    unregister_chrdev_region(devId, 2);
    return 0;
}

module_init(hello_init);
module_exit(hello_exit);
