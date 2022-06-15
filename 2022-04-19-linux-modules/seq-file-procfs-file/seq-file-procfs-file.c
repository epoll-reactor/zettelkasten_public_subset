/*
 * seq-file-procfs-file.c - Creates /proc file that uses
 * seq_file library.
 */
#include <linux/kernel.h>   // pr_info()
#include <linux/module.h>   // module API
#include <linux/proc_fs.h>  // procfs API
#include <linux/seq_file.h> // seq_file API
#include <linux/version.h>  // versions macros

MODULE_LICENSE("GPL");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAS_PROC_OPS
#endif

#define PROC_NAME "iter"

/* This function is called at the beginning of a sequence.
 * E.g, when
 * * the /proc file is read (first time);
 * * after the function stop (end of the sequence).
 */
static void *module_seq_start(struct seq_file *seq, loff_t *offset)
{
	static size_t counter = 0;

	// Beginning a new sequence?
	if (*offset == 0)
		// Yes, so return non-NULL value to begin the sequence.
		return &counter;

	// No, so it is the end of the sequence, return end to stop reading.
	*offset = 0;
	return NULL;
}

/* Called after the beginning of a sequence.
 * Called, until it returns NULL.
 */
static void *module_seq_next(struct seq_file *seq, void *ptr, loff_t *offset)
{
	(*(size_t *)ptr)++;
	(*offset)++;
	return NULL;
}

// Called at the end of sequence.
static void module_seq_stop(struct seq_file * /* unused */, void * /* unused */)
{
	// Nothing to do, we use a static value in start().
}

// Called for each step of sequence.
static int module_seq_show(struct seq_file *seq, void *ptr)
{
	loff_t *pos = (loff_t *)ptr;
	seq_printf(seq, "%Ld\n", *pos);
	return 0;
}

static struct seq_operations seq_ops = {
	.start = module_seq_start,
	.next  = module_seq_next,
	.stop  = module_seq_stop,
	.show  = module_seq_show
};

static int module_proc_open(struct inode * /* unused */, struct file *file)
{
	return seq_open(file, &seq_ops);
}

#ifdef HAS_PROC_OPS
static const struct proc_ops file_ops = {
	.proc_open    = module_proc_open,
	.proc_read    = seq_read,
	.proc_lseek   = seq_lseek,
	.proc_release = seq_release
};
#else
static const struct file_operations file_ops = {
	.open    = module_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};
#endif

static int __init procfs_init(void)
{
	struct proc_dir_entry *entry;

	entry = proc_create(PROC_NAME, 0, NULL, &file_ops);
	if (entry == NULL) {
		remove_proc_entry(PROC_NAME, NULL);
		pr_debug("error: could not initialize /proc/%s\n", PROC_NAME);
		return -ENOMEM;
	}

	return 0;
}

static void __exit procfs_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_debug("/proc/%s removed\n", PROC_NAME);
}

module_init(procfs_init);
module_exit(procfs_exit);
