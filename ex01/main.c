/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hello.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ag <agiraude@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:44:14 by ag                #+#    #+#             */
/*   Updated: 2023/08/25 13:57:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "Hello world !\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
