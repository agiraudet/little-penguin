#define F_ID_LOGIN "agiraude"
#define F_ID_LEN 8

static ssize_t f_id_read(struct file *file_pointer, char __user *buffer, size_t count, loff_t *ppos)
{
	char					*login = F_ID_LOGIN + *ppos;
	size_t				seeker = F_ID_LEN - *ppos;
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

static ssize_t f_id_write(struct file *file_pointer, const char __user *buffer, size_t count, loff_t *ppos)
{
	char	cmp_buf[F_ID_LEN + 1] = {0};
	char	*login = F_ID_LOGIN;

	if (count != F_ID_LEN + 1)
		return -EINVAL;
	if (copy_from_user(cmp_buf, buffer, count) != 0)
		return -EINVAL;
	for (size_t i = 0; i < F_ID_LEN; i++) {
		if (cmp_buf[i] != login[i])
			return -EINVAL;
	}
	return count;
}

static struct file_operations f_id_ops = {
	.owner = THIS_MODULE,
	.read = f_id_read,
	.write = f_id_write,
};
