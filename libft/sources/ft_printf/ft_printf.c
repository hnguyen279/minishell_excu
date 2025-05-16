/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:42:12 by trpham            #+#    #+#             */
/*   Updated: 2025/01/17 13:30:52 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static int	ft_informat(char c);
static int	ft_process(char c, va_list args, int fd);

int	ft_printf_fd(int fd, const char	*str, ...)
{
	va_list	args;
	int		i;
	int		count;

	if (!str)
		return (-1);
	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%' && ft_informat(str[i + 1]) == 1)
		{
			count += ft_process(str[i + 1], args, fd);
			i += 2;
		}
		else if (str[i] == '%' && str[i + 1] == '\0')
			return (va_end(args), -1);
		else
			count += ft_putchar_printf(str[i++], fd);
	}
	va_end(args);
	return (count);
}

static int	ft_informat(char c)
{
	char	*s;

	s = "cspdiuxX%";
	while (*s)
	{
		if (*s == c)
			return (1);
		s++;
	}
	return (0);
}

static int	ft_process(char c, va_list args, int fd)
{
	int	count;

	count = 0;
	if (c == 'c')
		count = (ft_putchar_printf(va_arg(args, int), fd));
	else if (c == 's')
		count = (ft_putstr_printf(va_arg(args, char *), fd));
	else if (c == 'd' || c == 'i')
		count = (ft_putnbr_printf(va_arg(args, int), fd));
	else if (c == 'u')
		count = (ft_putnbr_unsigned(va_arg(args, unsigned int), fd));
	else if (c == 'x' || c == 'X')
		count = (ft_puthex_fd(va_arg(args, unsigned int), fd, c));
	else if (c == 'p')
		count = (ft_putptr_fd(va_arg(args, void *), fd));
	else if (c == '%')
		count = (ft_putchar_printf('%', fd));
	return (count);
}
