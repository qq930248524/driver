#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static struct cdev chr_dev;	//定义一个字符设备对象
static dev_t ndev;		//字符设备节点的设备号

static int chr_open(struct inode *nd, struct file *file)
{
	int major = MAJOR(nd->i_rdev);
	int minor = MINOR(nd->i_rdev);
	printk("chr_open, major=%d, minor=%d\n", major, minor);
	return 0;
}

static ssize_t chr_read(struct file *f, char __user *u, size_t sz, loff_t *off)
{
	printk("In the chr_read() fun!\n");
	return 0;
}

struct file_operations chr_ops = 
{
	.owner = THIS_MODULE,
	.open = chr_open,
	.read = chr_read,
};

static int demo_init(void)
{
	int ret;
	//struct cdev
	cdev_init(&chr_dev, &chr_ops);
	//struct dev_t
	ret = alloc_chrdev_region(&ndev, 0, 1, "chr_dev");
	if(ret < 0){
		return ret;
	}
	printk("demo_init() major=%d, minor=%d\n", MAJOR(ndev), MINOR(ndev));

	ret = cdev_add(&chr_dev, ndev, 1);
	if(ret < 0){
		return ret;
	}

	return 0;
}

static int demo_exit(void)
{
	printk("Removing chr_dev modules! \n");
	cdev_del(&chr_dev);
	unregister_chrdev_region(ndev, 1);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZJY");

