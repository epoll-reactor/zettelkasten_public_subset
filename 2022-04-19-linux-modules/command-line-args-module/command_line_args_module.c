/*
 * command_line_args_module.c - Demonstrates command line argument passing to a module.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

static short int my_short    = 1;
static int       my_int      = 420;
static long      my_long     = 9999;
static char     *my_string   = "blah";
static int       my_array[2] = { 400, 420 };
static int       arr_argc    = 0;

/* module_param(foo, int, 0000)
 * The first param  - parameter name.
 * The second param - it's data type.
 * The third param  - permission bits in sysfs.
 *
 * NOTE: For some reason, variable description does not works,
 *       maybe it now deprecated.
 */
module_param(my_short, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
// MODULE_PARAM_DESC(my_short, "A short integer");

module_param(my_int, int,  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
// MODULE_PARAM_DESC(my_int, "An integer");

module_param(my_long, long, S_IRUSR);
// MODULE_PARAM_DESC(my_long, "A long integer");

module_param(my_string, charp, 0000);
// MODULE_PARAM_DESC(my_string, "A character string");

/* module_param_array(foo, int, num, 0000)
 * The first param  - parameter name.
 * The second param - it's data type.
 * The third param  - pointer to variable that will store arguments array length.
 * The fourth param - permission bits in sysfs.
 */
module_param_array(my_array, int, &arr_argc, 0000);
// MODULE_PARAM_DESC(my_array, "An array of integers");

static int __init command_line_args_init(void)
{
	int i;

	pr_info("Hello from command line args example resolver\n");
	pr_info("my_short  is %hd\n", my_short);
	pr_info("my_int    is %d\n", my_int);
	pr_info("my_long   is %ld\n", my_long);
	pr_info("my_string is %s\n", my_string);

	for (i = 0; i < ARRAY_SIZE(my_array); ++i)
		pr_info("my_array[%d] = %d\n", i, my_array[i]);

	pr_info("Got %d arguments for my_array\n", arr_argc);
	return 0;
}

static void __exit command_line_args_exit(void)
{
	pr_info("Goodbye from command line args example resolver\n");
}

module_init(command_line_args_init);
module_exit(command_line_args_exit);
