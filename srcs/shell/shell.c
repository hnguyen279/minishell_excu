/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:46:42 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 23:12:09 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	shell_cleanup(t_shell *mshell)
{
	if (!mshell)
		return ;
	if (mshell->envp)
		env_free(mshell);
}

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
	if (lvl < 0 || lvl > 999)
		lvl = 1;
	return (lvl);
}

static int	init_shlvl_env(t_shell *mshell)
{
	int		shlvl;
	char	*shlvl_str;

	shlvl = calculate_next_shlvl(mshell);
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
	{
		fd_printf_fd(2, "minishell: init_shell: malloc failed (SHLVL)\n");
		return (1);
	}
	if (env_add(mshell, "SHLVL", shlvl_str) != 0)
	{
		free(shlvl_str);
		fd_printf_fd(2, "minishell: init_shell: failed to set SHLVL\n");
		return (1);
	}
	free(shlvl_str);
	return (0);
}

static int	init_pwd_env(t_shell *mshell)
{
	char	*cwd;

	if (!env_find_value(mshell, "OLDPWD"))
		env_add(mshell, "OLDPWD", NULL);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		fd_printf_fd(2, "minishell: getcwd %s\n", strerror(errno));
		return (1);
	}
	if (env_add(mshell, "PWD", cwd) != 0)
	{
		free(cwd);
		fd_printf_fd(2, "minishell: init_shell: failed to set PWD\n");
		return (1);
	}
	free(cwd);
	return (0);
}

int	init_shell(t_shell *mshell, char **envp)
{
	ft_memset(mshell, 0, sizeof(t_shell));
	mshell->envp = env_dup(envp);
	if (!mshell->envp)
		return (error_msg(mshell, "init_shell: failed to duplicate environment",
				0));
	if (init_shlvl_env(mshell))
		return (error_msg(mshell, "init_shell: failed to initialize SHLVL", 0));
	if (init_pwd_env(mshell))
		return (error_msg(mshell, "init_shell: failed to initialize PWD/OLDPWD",
				0));
	mshell->exit_code = 0;
	return (0);
}
