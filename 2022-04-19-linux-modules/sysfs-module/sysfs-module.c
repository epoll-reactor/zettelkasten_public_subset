/*
 * sysfs-module.c - Sysfs example.
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");

static struct kobject *sysfs_object;

// The variable we want to be able to change.
static int sysfs_value = 0;

static ssize_t sysfs_value_show(
	struct kobject        *kobj,
	struct kobj_attribute *attr,
	char                  *buf)
{
	return sprintf(buf, "%d\n", sysfs_value);
}

static ssize_t sysfs_value_store(
	struct kobject        *kobj,
	struct kobj_attribute *attr,
	char                  *buf,
	size_t                 count)
{
	sscanf(buf, "%du", &sysfs_value);
	return count;
}

static struct kobj_attribute sysfs_value_attribute =
	__ATTR(sysfs_value, 0660, sysfs_value_show, (void *)sysfs_value_store);

static int __init sysfs_module_init(void)
{
	int error = 0;

	// Note: name "sysfs_object" causes error while creation.
	sysfs_object = kobject_create_and_add("__sysfs_object", kernel_kobj);
	if (!sysfs_object)
		return -ENOMEM;

	error = sysfs_create_file(sysfs_object, &sysfs_value_attribute.attr);
	if (error)
		pr_info("sysfs_module: failed to create /sys/__sysfs_object\n");

	return error;
}

static void __exit sysfs_module_exit(void)
{
	pr_info("sysfs_module: exit success\n");
	kobject_put(sysfs_object);
}

module_init(sysfs_module_init);
module_exit(sysfs_module_exit);
