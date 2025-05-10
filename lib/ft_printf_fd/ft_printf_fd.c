/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 18:17:21 by thi-huon          #+#    #+#             */
/*   Updated: 2024/11/30 18:34:04 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

static int	ft_check_format(va_list args, char c, int fd)
{
	if (c == 'c')
		return (ft_putchar(va_arg(args, int), fd));
	else if (c == 's')
		return (ft_putstr(va_arg(args, char *), fd));
	else if (c == 'p')
		return (ft_putptr(va_arg(args, void *), fd));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(args, int), fd));
	else if (c == 'u')
		return (ft_putnbr_unsigned(va_arg(args, unsigned int), fd));
	else if (c == 'x')
		return (ft_putnbr_base(va_arg(args, unsigned int), "0123456789abcdef", fd));
	else if (c == 'X')
		return (ft_putnbr_base(va_arg(args, unsigned int), "0123456789ABCDEF", fd));
	else if (c == '%')
		return (ft_putchar('%', fd));
	return (-1);
}

static int	ft_printf_helper(int fd, const char *format, va_list args)
{
	int	len;
	int	len_check;
	int	i;

	len = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == '\0')
				return (-1);
			i++;
			len_check = ft_check_format(args, format[i], fd);
		}
		else
		{
			len_check = ft_putchar(format[i], fd);
		}
		if (len_check == -1)
			return (-1);
		len += len_check;
		i++;
	}
	return (len);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	int		total_len;
	va_list	args;

	if (!format)
		return (-1);
	va_start(args, format);
	total_len = ft_printf_helper(fd, format, args);
	va_end(args);
	return (total_len);
}
