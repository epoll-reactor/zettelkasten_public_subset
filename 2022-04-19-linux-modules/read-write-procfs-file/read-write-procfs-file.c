
/*
 * read-write-procfs-file.c - file in /proc that handles read and write operations.
 */
#include <linux/kernel.h>  // pr_info()
#include <linux/module.h>  // module API
#include <linux/proc_fs.h> // procfs API
#include <linux/uaccess.h> // copy_from_user()
#include <linux/version.h> // version macros

MODULE_LICENSE("GPL");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME "1k_buffer"

// This structure holds information about the /proc file.
static struct proc_dir_entry *proc_file;

// The buffer used to store character for this module.
static char procfs_buf[PROCFS_MAX_SIZE];

// The size of the buffer.
static size_t procfs_buf_size = 0;

// Called when the /proc file is read.
static ssize_t procfile_read(
	struct file *file,
	char __user *buf,
	size_t       buf_len,
	loff_t      *offset)
{
	char str[] = "Hello, World\n";
	size_t str_len = sizeof(str);

	if (*offset >= str_len || copy_to_user(buf, str, str_len)) {
		pr_info("/proc/%s: copy_to_user failed\n", PROCFS_NAME);
		return 0;
	}

	pr_info("/proc/%s: read %s\n", PROCFS_NAME, file->f_path.dentry->d_name.name);
	*offset += str_len;

	return str_len;
}

// Called when the /proc file is written.
static ssize_t procfile_write(
	struct file       *file,
	const char __user *buf,
	size_t             buf_len,
	loff_t            *offset)
{
	procfs_buf_size = buf_len;
	if (procfs_buf_size > PROCFS_MAX_SIZE) {
		procfs_buf_size = PROCFS_MAX_SIZE;
	}

	if (copy_from_user(procfs_buf, buf, procfs_buf_size)) {
		pr_info("/proc/%s: copy_from_user failed", PROCFS_NAME);
		return -EFAULT;
	}

	procfs_buf[procfs_buf_size & (PROCFS_MAX_SIZE - 1)] = '\0';
	pr_info("/proc/%s: write %s\n", PROCFS_NAME, procfs_buf);

	return procfs_buf_size;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
	.proc_write = procfile_write
};
#else
static const struct file_operations proc_file_fops = {
	.read = procfile_read,
	.write = procfile_write
};
#endif

static int __init procfs_init(void)
{
	proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);
	if (proc_file == NULL) {
		pr_alert("/proc/%s: could not initialize file\n", PROCFS_NAME);
		return -ENOMEM;
	}

	pr_info("/proc/%s: created\n", PROCFS_NAME);
	return 0;
}

static void __exit procfs_exit(void)
{
	proc_remove(proc_file);
	pr_info("/proc/%s: removed\n", PROCFS_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
