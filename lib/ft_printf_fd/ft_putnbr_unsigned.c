/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:23:49 by thi-huon          #+#    #+#             */
/*   Updated: 2024/12/07 16:23:51 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

int	ft_putnbr_unsigned(unsigned int nbr, int fd)
{
	char	num;
	int		len;
	int		recursive_len;

	len = 0;
	if (nbr >= 10)
	{
		recursive_len = ft_putnbr_unsigned(nbr / 10, fd);
		if (recursive_len == -1)
			return (-1);
		len += recursive_len;
	}
	num = (nbr % 10) + '0';
	if (ft_putchar(num, fd) == -1)
		return (-1);
	return (len + 1);
}
