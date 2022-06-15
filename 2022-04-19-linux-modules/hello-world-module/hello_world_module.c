/*
 * hello_world_module.c - The simplest kernel module.
 */
#include <linux/kernel.h> /* Needed for pr_info(). */
#include <linux/module.h> /* Needed by all modules. */

MODULE_LICENSE("GPL");

int init_module(void)
{
	pr_info("Hello World 1.\n");

	/* A non 0 return means init_module failed; module can't be loaded. */
	return 0;
}

void cleanup_module(void)
{
	pr_info("Goodbye World 1.\n");
}
