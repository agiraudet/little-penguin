#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

static struct proc_dir_entry *mnt_entry;
static int mnt_open(struct inode *inode, struct file *file);
static void look_for_mounts(struct seq_file *s, struct list_head *d_parent);

static const struct proc_ops mnt_proc_ops = {
	.proc_open = mnt_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release
};

char path[PAGE_SIZE];

static void on_entry(struct seq_file *s, struct dentry *entry)
{
		size_t len = strlen(path);
		
		strncat(path, "/", PAGE_SIZE - len);
		strncat(path, entry->d_name.name, PAGE_SIZE - len - 1);
		if (entry->d_flags & DCACHE_MOUNTED)
			seq_printf(s, "%s\t%s\n", entry->d_name.name, path);
		else
			look_for_mounts(s, &entry->d_subdirs);
		path[len] = 0;
}

static void look_for_mounts(struct seq_file *s, struct list_head *d_parent)
{
	struct dentry *curdentry;

	list_for_each_entry(curdentry, d_parent, d_child)
		on_entry(s, curdentry);
}

static int mnt_show(struct seq_file *s, void *p)
{
	path[0] = 0;
	
	seq_printf(s, "%s\t%s\n", "root", current->fs->root.dentry->d_name.name);
	look_for_mounts(s, &current->fs->root.mnt->mnt_root->d_subdirs);
	return 0;
}

static int mnt_open(struct inode *inode, struct file *file)
{
	return single_open(file, mnt_show, 0);
}

static int mnt_init(void)
{
	mnt_entry = proc_create("mymounts", 0, NULL, &mnt_proc_ops);
	return 0;
}

static void mnt_exit(void)
{
	proc_remove(mnt_entry);
}

module_init(mnt_init);
module_exit(mnt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("agiraude");
