/*
 * read-only-device.c - Creates a read-only char device that says how many
 * times you read from it.
 *
 * NOTE: device file should be created manually with mknod and next
 * simply deleted with rm. This is shown in usage.sh.
 */
#include <linux/device.h> // device_create()
#include <linux/init.h>   // __init, __exit
#include <linux/fs.h>     // file_operations, (un?)register_chrdev()
#include <linux/kernel.h> // pr_info()
#include <linux/module.h> // module API

MODULE_LICENSE("GPL");

static int rdonly_device_open   (struct inode *, struct file *);
static int rdonly_device_release(struct inode *, struct file *);
static ssize_t rdonly_device_read (struct file *,       char __user *, size_t, loff_t *);
static ssize_t rdonly_device_write(struct file *, const char __user *, size_t, loff_t *);

#define DEVICE_NAME "read_only_device" // Dev name as it apperas in /proc/devices.
#define BUF_LEN     8192               // Max length of the message from the device.

static int major; // Major number assigned to device driver.

enum {
	CDEV_NOT_USED       = 0,
	CDEV_EXCLUSIVE_OPEN = 1
};

// Is device open? Used to prevent multiple access to device.
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg_buf[BUF_LEN]; // The message the device will give when asked.

// High-level abstraction of device. Classes allow userspace to work with
// devices based on what they do, rather than how they are connected or
// how they work.
static struct class *device;

static struct file_operations rdonly_device_fops = {
	.owner   = THIS_MODULE,
	.read    = rdonly_device_read,
	.write   = rdonly_device_write,
	.open    = rdonly_device_open,
	.release = rdonly_device_release 
};

static int __init rdonly_device_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &rdonly_device_fops);

	if (major < 0) {
		pr_alert("Registering char device failed with %d\n", major);
		return major;
	}

	pr_info("Major number %d was assigned to device", major);
	device = class_create(THIS_MODULE, DEVICE_NAME);

	if (device) {
		pr_info("Device /dev/%s already exists.", DEVICE_NAME);
		return 0;
	}
	
	device_create(device, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	pr_info("Device created on /dev/%s\n", DEVICE_NAME);

	return 0;
}

static void __exit rdonly_device_exit(void)
{
	device_destroy(device, MKDEV(major, 0));
	class_destroy(device);

	// Unregister our device (remove note in /proc/devices).
	unregister_chrdev(major, DEVICE_NAME);

	pr_info("Device deleted on /dev/%s\n", DEVICE_NAME);
}

// Called when a process tries to open the device file, like "sudo cat /dev/read_only_device".
static int rdonly_device_open(struct inode * /* unused */, struct file * /* unused */)
{
	static int counter = 0;

	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
		return -EBUSY;

	sprintf(msg_buf, "/dev/%s was accessed %d times\n", DEVICE_NAME, counter++);
	pr_info("%s", msg_buf);

	// Increment the usage count.
	try_module_get(THIS_MODULE);

	return 0;
}

// Called when a process closes the device file.
static int rdonly_device_release(struct inode * /* unused */, struct file * /* unused */)
{
	// We're now ready for our next caller.
	atomic_set(&already_open, CDEV_NOT_USED);

	// Decrement the usage count, or else once you opened the file, you
	// will get rid of the module.
	module_put(THIS_MODULE);

	pr_info("/dev/%s was closed\n", DEVICE_NAME);

	return 0;
}

// Called when a process, which already opened the dev file, attempts to read from it.
static ssize_t rdonly_device_read(
	struct file * /* unused */,
	char __user *buffer,
	size_t       length,
	loff_t      *offset)
{
	// Number of bytes actually written to the buffer.
	size_t bytes_read = 0;
	const char *msg_ptr = msg_buf;

	if (!*(msg_ptr + *offset)) { // We are at the end of message.
		*offset = 0; // Reset the offset.
		return 0; // Signify end of file.
	}

	msg_ptr += *offset;

	// Actually put the data into the buffer.
	while (length && *msg_ptr) {
		// The buffer is in the user data segment, not the kernel
		// segment so "*" assignment won't work. We have to use put_user
		// which copies data from the kernel data segment to the user
		// data segment.
		put_user(*(msg_ptr++), buffer++);
		length--;
		bytes_read++;
	}

	*offset += bytes_read;

	// Most read functions return the number of bytes put into the buffer.
	return bytes_read;
}

// Called when a process writes to dev file: echo "Hi" > /dev/rdonly.
static ssize_t rdonly_device_write(
	struct file       * /* unused */,
	const char __user * /* unused */,
	size_t              /* unused */,
	loff_t            * /* unused */)
{
	pr_info("Sorry, write operation is not supported.\n");

	return -EINVAL;
}

module_init(rdonly_device_init);
module_exit(rdonly_device_exit);

#undef DEVICE_NAME
#undef BUF_LEN
