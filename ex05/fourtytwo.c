#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");

#define FT_LOGIN "agiraude"
#define FT_LEN 8

static ssize_t ft_read(struct file *file_pointer, char __user *buffer, size_t count, loff_t *ppos);
static ssize_t ft_write(struct file *file_pointer, const char __user *buffer, size_t count, loff_t *ppos);

static struct file_operations ft_dev_ops = {
	.owner = THIS_MODULE,
	.read = ft_read,
	.write = ft_write,
};


struct miscdevice ft_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fourtytwo",
	.fops = &ft_dev_ops
};

static ssize_t ft_read(struct file *file_pointer, char __user *buffer, size_t count, loff_t *ppos)
{
	char					*login = FT_LOGIN + *ppos;
	size_t				seeker = FT_LEN - *ppos;
	size_t				cp_bytes;

	if (count < seeker)
		seeker = count;
	if (seeker <= 0)
		return 0;
	cp_bytes = copy_to_user(buffer, login, seeker);
	if (cp_bytes == seeker)
		return -EFAULT;
	*ppos = seeker - cp_bytes;
	return seeker - cp_bytes;
}

static ssize_t ft_write(struct file *file_pointer, const char __user *buffer, size_t count, loff_t *ppos)
{
	char	cmp_buf[FT_LEN + 1] = {0};
	char	*login = FT_LOGIN;

	if (count != FT_LEN + 1)
		return -EINVAL;
	if (copy_from_user(cmp_buf, buffer, count) != 0)
		return -EINVAL;
	for (size_t i = 0; i < FT_LEN; i++) {
		if (cmp_buf[i] != login[i])
			return -EINVAL;
	}
	return count;
}


static int ft_init(void)
{
	printk(KERN_ALERT "Registering fourtytwo\n");
	return misc_register(&ft_dev);
}

static void ft_exit(void)
{
	printk(KERN_ALERT "De-registering fourtytwo\n");
	return misc_deregister(&ft_dev);
}

module_init(ft_init);
module_exit(ft_exit);
