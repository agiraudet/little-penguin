#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/fs_struct.h>

static int __init my_module_init(void)
{
	struct dentry *curdentry;

	/* printk("%s   XX\n", current->fs->root.mnt->mnt_root->d_name.name); */
	printk("%s   XX\n", current->fs->root.dentry->d_name.name);

	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs,
			d_child){
		if ( curdentry->d_flags & DCACHE_MOUNTED)
			printk("%s   %s\n", curdentry->d_name.name, curdentry->d_parent->d_name.name);
	}
	return 0;
}

static void __exit my_module_exit(void)
{
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("agiraude");
