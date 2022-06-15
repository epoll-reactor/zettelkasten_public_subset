/*
 * initdata_variable_module.c - Illustrating the __init,
 * __initdata and __exit macros.
 */
#include <linux/init.h>   /* Needed for the __init, __exit macros. */
#include <linux/kernel.h> /* Needed for pr_info() */
#include <linux/module.h> /* Needed by all modules */

MODULE_LICENSE("GPL");

static int data __initdata = 187;

static int __init initdata_variable_init(void)
{
	pr_info("initdata_variable module started with arg %d\n", data);
	return 0;
}

static void __exit initdata_variable_exit(void)
{
	pr_info("initdata_variable module terminated\n");
}

module_init(initdata_variable_init);
module_exit(initdata_variable_exit);
