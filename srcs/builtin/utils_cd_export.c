/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:37:56 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:51:58 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	update_pwd(t_shell *mshell)
{
	char		*cwd;
	const char	*oldpwd;

	oldpwd = env_find_value(mshell, "PWD");
	if (oldpwd)
		env_add(mshell, "OLDPWD", oldpwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		env_add(mshell, "PWD", cwd);
		free(cwd);
	}
	mshell->exit_code = 0;
}

int	export_with_equal(t_shell *mshell, const char *arg, char *equal)
{
	char	*key;

	key = ft_substr(arg, 0, equal - arg);
	if (!key)
	{
		ft_printf_fd(2, "minishell: export: malloc failed\n");
		return (1);
	}
	env_remove(mshell, key);
	if (env_add(mshell, key, equal + 1) != 0)
	{
		free(key);
		return (1);
	}
	free(key);
	return (0);
}

int	handle_plus_equal_case(t_shell *mshell, const char *arg,
		char *plus_equal)
{
	char	*key;

	key = ft_substr(arg, 0, plus_equal - arg);
	if (!key)
		return (1);
	if (!export_is_valid_key(key))
	{
		free(key);
		ft_printf_fd(2, "minishell: export: `%s`: not a valid identifier\n",
			arg);
		return (1);
	}
	free(key);
	return (export_plus_equal(mshell, arg, plus_equal));
}

int	handle_equal_case(t_shell *mshell, const char *arg, char *equal)
{
	char	*key;

	key = ft_substr(arg, 0, equal - arg);
	if (!key)
		return (1);
	if (!export_is_valid_key(key))
	{
		free(key);
		ft_printf_fd(2, "minishell: export: `%s`: not a valid identifier\n",
			arg);
		return (1);
	}
	free(key);
	return (export_with_equal(mshell, arg, equal));
}
