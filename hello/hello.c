#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>

#include "hello.h"

MODULE_LICENSE("Dual BSD/GPL");

int hello_major = 0;
int hello_minor = 4;

module_param(hello_major, int, S_IRUGO);
module_param(hello_minor, int, S_IRUGO);

dev_t devId;

static void  __init hello_init(void)
{
    dev_t dev;

    if(hello_major){
        dev = MKDEV(hello_major, hello_mainor);
        if(register_chrdev_region() != 0)   goto fail;
    }

    if(alloc_chrdev_region(&devId, 0, 2, "zjy_") != 0)
    //if(register_chrdev_region(444, 2, "zjy_1") != 0)
        goto fail;
    printk(KERN_ALERT "init howman = %d, whom = %s\n", howmany, whom);
    return ;

fail:
    unregister_chrdev_region(devId, 2);
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "exit\n");
    unregister_chrdev_region(devId, 2);
    return ;
}

module_init(hello_init);
module_exit(hello_exit);
