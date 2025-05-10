/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 12:49:18 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/02 12:57:51 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

static	void	ft_recursive_nbr(long int n, int fd)
{
	char	num;

	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
	{
		ft_recursive_nbr(n / 10, fd);
	}
	num = n % 10 + '0';
	write(fd, &num, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	long int	nb;

	nb = n;
	if (nb == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	ft_recursive_nbr(nb, fd);
}
