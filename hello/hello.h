#ifndef _SCULL_
#define _SCULL_


#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 4
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

extern int scull_major;
extern int scull_nr_devs;
extern int scull_qset;
extern int scull_quantum;

struct scull_qset{
    struct scull_qset *next;
    void **data;
};
struct scull_dev{
    struct scull_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned long access_key;
    struct semaphore sem;
    struct cdev cdev;
};

int     scull_trim(struct scull_dev* dev);
int     scull_open(struct inode *inode, struct file *filep);
ssize_t scull_read(struct file *filep, char __user *buf, size_t count, loff_t *offp);
ssize_t scull_write(struct file *filep, const char __user *buf, size_t count, loff_t *offp);


#endif
