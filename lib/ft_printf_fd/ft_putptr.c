/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:40:34 by thi-huon          #+#    #+#             */
/*   Updated: 2024/12/07 16:40:45 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

int	ft_putptr(void *ptr, int fd)
{
	int	len;
	int	base_len;

	if (!ptr)
		return (write(1, "(nil)", 5));
	len = ft_putstr("0x", fd);
	if (len == -1)
		return (-1);
	base_len = ft_putnbr_base((unsigned long long)ptr, "0123456789abcdef", fd);
	if (base_len == -1)
		return (-1);
	return (len + base_len);
}
