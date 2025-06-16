/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:31:27 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/16 14:49:01 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	sign_and_limit(const char *str, size_t *i, int *sign,
		unsigned long *limit)
{
	*sign = 1;
	*limit = LONG_MAX;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
		{
			*sign = -1;
			*limit = (unsigned long)LONG_MAX + 1;
		}
		(*i)++;
	}
}

static int	digits_with_overflow(const char *str, size_t *i,
		unsigned long *result, unsigned long limit)
{
	int	digit;

	*result = 0;
	while (ft_isdigit(str[*i]))
	{
		digit = str[*i] - '0';
		if (*result > (limit - digit) / 10)
			return (0);
		*result = *result * 10 + digit;
		(*i)++;
	}
	return (1);
}

static int	ft_atol_check(const char *str, long *out)
{
	size_t			i;
	int				sign;
	unsigned long	result;
	unsigned long	limit;

	i = 0;
	if (!str || !out)
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign_and_limit(str, &i, &sign, &limit);
	if (!ft_isdigit(str[i]))
		return (0);
	if (!digits_with_overflow(str, &i, &result, limit))
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] != '\0')
		return (0);
	*out = (long)(sign * result);
	return (1);
}

static int	to_valid_exit_code(long num)
{
	num %= 256;
	if (num < 0)
		num += 256;
	return ((int)num);
}

void	builtin_exit(t_shell *mshell, char **token)
{
	long	code;

	code = 0;
	if (mshell->token_list->type == PIPE)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (token[1] && !ft_atol_check(token[1], &code))
	{
		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n",
			token[1]);
		mshell->exit_code = 2;
	}
	else if (token[1] && token[2])
	{
		ft_printf_fd(2, "minishell: exit: too many arguments\n");
		mshell->exit_code = 1;
		return ;
	}
	else if (token[1])
		mshell->exit_code = to_valid_exit_code(code);
	shell_cleanup(mshell);
	exit(mshell->exit_code);
}
