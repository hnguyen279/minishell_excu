/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:30:52 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/17 19:43:08 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	builtin_env(t_shell *mshell, char **token)
{
	if (!mshell || !token || !token[0])
	{
		ft_printf_fd(2, "minishell: builtin_env: Invalid parameter(s)\n");
		return (1);
	}
	if (token[1])
	{
		if (token[1][0] == '-')
		{
			ft_printf_fd(2, "minishell: env: unrecognized option '%s'\n", token[1]);
			return (125);
		}
		ft_printf_fd(2, "minishell: env: '%s': No such file or directory\n", token[1]);
		return (127);
	}
	env_print(mshell->envp);
	return (0);
}
