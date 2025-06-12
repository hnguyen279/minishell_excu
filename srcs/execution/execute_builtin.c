/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:20:01 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/10 22:14:25 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "export") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_shell *mshell, char **token)
{
	if (ft_strcmp(token[0], "cd") == 0)
		return (builtin_cd(mshell, token));
	else if (ft_strcmp(token[0], "echo") == 0)
		return (builtin_echo(mshell, token));
	else if (ft_strcmp(token[0], "pwd") == 0)
		return (builtin_pwd(mshell));
	else if (ft_strcmp(token[0], "unset") == 0)
		return (builtin_unset(mshell, token));
	else if (ft_strcmp(token[0], "env") == 0)
		return (builtin_env(mshell, token));
	else if (ft_strcmp(token[0], "export") == 0)
		return (builtin_export(mshell, token));
	else if (ft_strcmp(token[0], "exit") == 0)
	{
		builtin_exit(mshell, token);
		return (mshell->exit_code);
	}
	else
		return (1);
}
