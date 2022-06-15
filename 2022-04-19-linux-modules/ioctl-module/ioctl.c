/*
 * ioctl.c - Example ioctl usage.
 */
#include <linux/module.h> // module API
#include <linux/kernel.h> // pr_info()
#include <linux/fs.h>     // file_operations(), alloc_chrdev_region()
#include <linux/cdev.h>   // cdev_init(), ...
#include <linux/device.h> // device_create(), ...

MODULE_LICENSE("GPL");

typedef struct ioctl_device_data_t {
	struct cdev cdev;
	uint8_t     data;
} ioctl_device_data_t;

static ioctl_device_data_t device_data;
static struct class       *cl;
static dev_t               dev;

static int ioctl_device_open(struct inode * /* unused */, struct file * /* unused */)
{
	return 0;
}

static int ioctl_device_release(struct inode * /* unused */, struct file * /* unused */)
{
	return 0;
}

static long ioctl_device_ioctl(
	struct file  *file,
	unsigned int  cmd,
	unsigned long arg)
{
	switch (cmd) {
	case 0xFFAB:
		pr_info("ioctl_device: %d, %s\n", task_pid_nr(current), current->comm);
		break;

	case 0xFFAC:
		if (arg == 8) {
			pr_info("ioctl_device: argument = %ld.\n", arg);
		} else {
			pr_warn("ioctl_device: wrong argument.\n");
			return -1;
		}
		break;
	}
	return 0;
}

const struct file_operations fops = {
	.owner          = THIS_MODULE,
	.open           = ioctl_device_open,
	.release        = ioctl_device_release,
	.unlocked_ioctl = ioctl_device_ioctl
};

static int __init ioctl_device_init(void)
{
	int ret;
	struct device *dev_ret;

	ret = alloc_chrdev_region(&dev, 0, 1, "ioctl_device");
	if (ret < 0)
		return ret;

	// Create class for sysfs.
	cl = class_create(THIS_MODULE, "char_driver");
	if (IS_ERR(cl)) {
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(cl);
	}

	// Create device for sysfs.
	dev_ret = device_create(cl, NULL, dev, NULL, "ioctl_device");
	if (IS_ERR(dev_ret)) {
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(dev_ret);
	}

	// Create character device.
	cdev_init(&device_data.cdev, &fops);
	ret = cdev_add(&device_data.cdev, dev, 1);
	if (ret < 0) {
		device_destroy(cl, dev);
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return ret;
	}

	pr_info("ioctl_device: initialized.\n");
	return 0;
}

static void __exit ioctl_device_exit(void)
{
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&device_data.cdev);
	unregister_chrdev_region(dev, 1);
}

module_init(ioctl_device_init);
module_exit(ioctl_device_exit);
