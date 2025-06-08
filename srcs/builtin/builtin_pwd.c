/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:31:44 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 22:13:19 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	parse_pwd_args(char **token)
{
	int	i;

	i = 1;
	while (token[i])
	{
		if (token[i][0] == '-' && token[i][1] != '\0')
		{
			ft_printf_fd(2, "minishell: pwd: options aren't supported\n");
			ft_printf_fd(2, "pwd: usage: pwd\n");
			return (2);
		}
		i++;
	}
	return (0);
}

int	builtin_pwd(t_shell *mshell, char **token)
{
	char		*cwd;
	const char	*env_pwd;
	int			err;

	err = parse_pwd_args(token);
	if (err != 0)
		return (err);
	env_pwd = env_find_value(mshell, "PWD");
	if (env_pwd)
	{
		printf("%s\n", env_pwd);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
