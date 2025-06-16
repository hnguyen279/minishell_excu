/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:57:09 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 01:43:36 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	calculate_next_shlvl(t_shell *mshell)
{
	char	*val;
	int		lvl;
	int		i;

	val = env_find_value(mshell, "SHLVL");
	lvl = 1;
	if (val && val[0] != '\0')
	{
		i = 0;
		while (val[i] && ft_isdigit(val[i]))
			i++;
		if (val[i] == '\0')
			lvl = ft_atoi(val) + 1;
	}
	if (lvl < 0)
		lvl = 1;
	if (lvl > 999)
	{
		ft_printf_fd(2, "minishell: warning: shell level (%d) too high, resetting to 1\n", lvl);
		lvl = 1;
	}
	return (lvl);
}

int	init_shlvl_env(t_shell *mshell)
{
	int		shlvl;
	char	*shlvl_str;

	shlvl = calculate_next_shlvl(mshell);
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
	{
		ft_printf_fd(2, "minishell: init_shell: malloc failed (SHLVL)\n");
		return (1);
	}
	if (env_add(mshell, "SHLVL", shlvl_str) != 0)
	{
		free(shlvl_str);
		ft_printf_fd(2, "minishell: init_shell: failed to set SHLVL\n");
		return (1);
	}
	free(shlvl_str);
	return (0);
}

int	init_pwd_env(t_shell *mshell)
{
	mshell->curr_pwd = getcwd(NULL, 0);
	if (!mshell->curr_pwd)
	{
		ft_printf_fd(2, "minishell: getcwd %s\n", strerror(errno));
		return (1);
	}
	if (env_add(mshell, "PWD", mshell->curr_pwd) != 0)
	{
		ft_printf_fd(2, "minishell: init_shell: failed to set PWD\n");
		return (1);
	}
	return (0);
}
