#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "hello.h"

MODULE_LICENSE("Dual BSD/GPL");


int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_qset = SCULL_QSET;
int scull_quantum = SCULL_QUANTUM;
static char *deviceName = "zjy_";

struct scull_dev *scull_devices;

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(deviceName, charp, S_IRUGO);

dev_t devId;

//empty out the scull device
int scull_trim(struct scull_dev *dev)
{
    struct scull_qset *next, *dptr;
    int qset = dev->qset;
    int i;

    for(dptr = dev->data; dptr; dptr = next){
        if(dptr->data){
            for(i=0; i<qset; i++){
                kfree(dptr->data[i]);
            }
            kfree(dptr->data);
            dptr->data = NULL;
        }
        next = dptr->next;
        kfree(dptr);
    }
    dev->data = NULL;
    dev->size = 0;
    dev->qset = scull_qset;
    dev->quantum = scull_quantum;
    return 0;
}
int scull_open(struct inode *inode, struct file *filep)
{
    struct scull_dev *dev;
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filep->private_data = dev;

    if((filep->f_flags & O_ACCMODE) == O_WRONLY){
        scull_trim(dev);
    }
    return 0;
}
struct file_operations scull_fops = {
    .owner = THIS_MODULE,
    .open   =   scull_open,
    /*
    .llseak =   scull_llseak,
    .read   =   scull_read,
    .write  =   scull_write,
    .release=   scull_release,
    */
};

static void scull_setup_cdev(struct scull_dev * dev, int index)
{
    int devno=MKDEV(scull_major, scull_minor+index);
    cdev_init(&dev->cdev, &scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &scull_fops;
    if(cdev_add(&dev->cdev, devno, 1) < 0){
        printk(KERN_NOTICE "Error add scull %d\n", devno);
    }
    
}
static int __init scull_init(void)
{
    int result = 0;
    int i;
    dev_t dev;

    //registe
    if(scull_major){
        dev = MKDEV(scull_major, scull_minor);
        if((result = register_chrdev_region(dev, scull_nr_devs, deviceName)) != 0)   goto fail;
    }else{
        if((result = alloc_chrdev_region(&dev, 0, scull_nr_devs, deviceName)) != 0)  goto fail;
        scull_major = MAJOR(dev);
    }
    
    //kmalloc struct scull_qset
    scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
    if(scull_devices == NULL){
        result = -ENOMEM;
        goto fail;
    }
    memset(scull_devices, 0, scull_nr_devs*sizeof(struct scull_dev));
    printk(KERN_ALERT "The length of scull is %d", sizeof(struct scull_dev));

    for(i = 0; i < scull_nr_devs; i++){
        scull_devices[i].quantum = SCULL_QUANTUM;
        scull_devices[i].qset = SCULL_QSET;
        scull_setup_cdev(&scull_devices[i], i);
    }

    return 0;
fail:
    unregister_chrdev_region(devId, 2);
    return result;
}

static void __exit scull_exit(void)
{
    printk(KERN_ALERT "exit\n");
    unregister_chrdev_region(devId, 2);
    return;
}

module_init(scull_init);
module_exit(scull_exit);
