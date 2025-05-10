/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:23:09 by thi-huon          #+#    #+#             */
/*   Updated: 2024/12/07 16:23:13 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

int	ft_putnbr_base(unsigned long long nbr, const char *base, int fd)
{
	int					len;
	unsigned long long	len_base;
	char				num;
	int					recursive_len;

	len = 0;
	len_base = 16;
	if (nbr >= len_base)
	{
		recursive_len = ft_putnbr_base(nbr / len_base, base, fd);
		if (recursive_len == -1)
			return (-1);
		len += recursive_len;
	}
	num = base[nbr % len_base];
	if (ft_putchar(num, fd) == -1)
		return (-1);
	return (len + 1);
}
