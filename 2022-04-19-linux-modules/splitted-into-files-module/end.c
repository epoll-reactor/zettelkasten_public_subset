/*
 * end.c - Illustration of multi-filed modules.
 */
#include <linux/init.h>   /* __init macros. */
#include <linux/kernel.h> /* We are doing kernel work */
#include <linux/module.h> /* Specifically, a module */

MODULE_LICENSE("GPL");

static void __exit splitted_into_files_exit(void)
{
	pr_info("Goodbye from splitted to files module\n");
}

module_exit(splitted_into_files_exit);
