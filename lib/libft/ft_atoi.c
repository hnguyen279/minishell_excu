/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:19:43 by thi-huon          #+#    #+#             */
/*   Updated: 2024/10/30 15:44:34 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

int	ft_atoi(const char *str)
{
	int				sign;
	long long int	num;
	int				i;

	sign = 1;
	num = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
		sign = -sign;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign > 0 && num > (LLONG_MAX - (str[i] - '0')) / 10)
			return (INT_MAX);
		if (sign < 0 && num > (-(LLONG_MIN + (str[i] - '0'))) / 10)
			return (INT_MIN);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(num * sign));
}
