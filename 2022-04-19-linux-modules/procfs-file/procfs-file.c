/*
 * procfs-file.c - Creates and deletes read-only file in /proc.
 */
#include <linux/kernel.h>  // pr_info()
#include <linux/module.h>  // module API
#include <linux/proc_fs.h> // procfs API
#include <linux/version.h> // version macros

MODULE_LICENSE("GPL");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_NAME "rdonly_file"

static struct proc_dir_entry *proc_file;

// Called when we read from /proc/rdonly_file. The content of buf is returned to
// the application which read it (for example, the cat command). The offset is
// the current position in the file. If the return value is not null, then this
// function is called again. So we should be careful with this, if it never returns
// zero, the read function is called endlessly.
static ssize_t procfile_read(
	struct file *file,
	char __user *buf,
	size_t       buf_len,
	loff_t      *offset)
{
	char string[] = "Hello, World\n";
	size_t string_len = sizeof(string);
	ssize_t ret = string_len;

	if (*offset >= string_len || copy_to_user(buf, string, string_len)) {
		pr_info("copy_to_user failed\n");
		ret = 0;
	} else {
		pr_info("procfile read %s\n", file->f_path.dentry->d_name.name);
		*offset += string_len;
	}

	return ret;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read
};
#else
static const struct file_operations proc_file_fops = {
	.read = procfile_read
};
#endif

static int __init procfs_init(void)
{
	proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);
	if (proc_file == NULL) {
		proc_remove(proc_file);
		pr_alert("Error: could not initialize /proc/%s\n", PROCFS_NAME);
		return -ENOMEM;
	}

	pr_info("/proc/%s created\n", PROCFS_NAME);
	return 0;
}

static void __exit procfs_exit(void)
{
	proc_remove(proc_file);
	pr_info("/proc/%s was removed\n", PROCFS_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
