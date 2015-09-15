#ifndef _SCULL_H_
#define _SCULL_H_
#include <linux/ioctl.h>

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 4    /* scull0 ~ scull3 */
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET  1000
#endif

struct scull_qset{
    void **data;
    struct scull_qset *next;
};
struct scull_dev{
    struct scull_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned int access_key;
    struct semaphore sem;
    struct cdev cdev;
};
#define TYPE(minor) ((minor)>>4) && 0xff)
#define NUM(minor)  ((minor) & 0xf)

extern int scull_major;     /* main.c */
extern int scull_nr_devs;   
extern int scull_quantum;
extern int scull_qset;

int      scull_trim(struct scull_dev *dev);
ssize_t  scull_read(struct file *filep,char __user *buf, size_t count,loff_t *offp);
ssize_t  scull_write(struct file *filep, const char __user *buff, size_t count,loff_t *offp);
loff_t   scull_llseek(struct file *filep,loff_t off,int whence);
int      scull_ioctl(struct inode *inode,struct file *filep,unsigned int cmd,unsigned long arg);

#define SCULL_IOC_MAXNR  14
#endif
