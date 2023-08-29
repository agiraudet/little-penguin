#include <linux/init.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include "f_id.c"
#include "f_foo.c"

struct dentry		*d_fortytwo = 0;

static int ft_debug_init(void)
{
	d_fortytwo = debugfs_create_dir("fortytwo", 0);
	if (d_fortytwo == ERR_PTR(-ENODEV)) {
		printk(KERN_ALERT "CONFIG_DEBUG_FS need to be set to y.\n");
		return 1;
	}
	if (!d_fortytwo) {
		printk(KERN_ALERT "debugfs_create_dir: Something went wrong.\n");
		return 1;
	}
	debugfs_create_file("id", 0666, d_fortytwo, 0, &f_id_ops);
	debugfs_create_u64("jiffies", 0444, d_fortytwo, (u64*)&jiffies);
	mutex_init(&foo_lock);
	foo_page = (char*)get_zeroed_page(GFP_KERNEL);
	debugfs_create_file("foo", 0644, d_fortytwo, foo_page, &f_foo_ops);
	return 0;
}

static void ft_debug_exit(void)
{
	if (foo_page)
		free_page((unsigned long int)foo_page);
	if (d_fortytwo)
		debugfs_remove_recursive(d_fortytwo);
}

module_init(ft_debug_init);
module_exit(ft_debug_exit);
MODULE_LICENSE("Dual BSD/GPL");
