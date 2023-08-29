static struct mutex foo_lock;

static char *foo_page = 0;

static ssize_t f_foo_write(struct file *file_pointer, const char __user *buffer,
		size_t count, loff_t *ppos)
{
	mutex_lock(&foo_lock);
	if (*ppos + count > PAGE_SIZE)
		count = PAGE_SIZE - *ppos;
	if (copy_from_user(foo_page + *ppos, buffer, count)) {
		mutex_unlock(&foo_lock);
		return -EFAULT;
	}
	*ppos += count;
	mutex_unlock(&foo_lock);
	return count;
}

static ssize_t f_foo_read(struct file *file_pointer, char __user *buffer,
		size_t count, loff_t *ppos)
{
	mutex_lock(&foo_lock);
	size_t remaining_bytes = PAGE_SIZE - *ppos;
	if (remaining_bytes <= 0)
		return 0;
	size_t rb = remaining_bytes < count ? remaining_bytes : count;
	if (copy_to_user(buffer, foo_page + *ppos, rb)) {
		mutex_unlock(&foo_lock);
		return -EFAULT;
	}
	*ppos += rb;
	mutex_unlock(&foo_lock);
	return rb;
}

static struct file_operations f_foo_ops = {
	.owner = THIS_MODULE,
	.read = f_foo_read,
	.write = f_foo_write,
};
