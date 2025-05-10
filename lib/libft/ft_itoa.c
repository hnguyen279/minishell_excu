/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 13:47:43 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/02 13:48:23 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

static int	ft_len_int(long int n)
{
	int	len_int;

	if (n <= 0)
		len_int = 1;
	else
		len_int = 0;
	while (n != 0)
	{
		n = n / 10;
		len_int++;
	}
	return (len_int);
}

char	*ft_itoa(int n)
{
	char		*dest;
	long int	nb;
	int			len_int;

	nb = n;
	len_int = ft_len_int(nb);
	dest = (char *)malloc(sizeof(char) * (len_int + 1));
	if (dest == NULL)
		return (NULL);
	dest[len_int] = '\0';
	if (nb == 0)
		dest[0] = '0';
	if (nb < 0)
	{
		dest[0] = '-';
		nb = -nb;
	}
	while (nb > 0)
	{
		len_int--;
		dest[len_int] = (nb % 10) + '0';
		nb = nb / 10;
	}
	return (dest);
}
