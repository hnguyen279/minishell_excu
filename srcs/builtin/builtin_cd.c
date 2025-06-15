/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:30:06 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/15 19:28:19 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	cd_to_home(t_shell *mshell)
{
	const char	*home;

	home = env_find_value(mshell, "HOME");
	if (!home)
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		mshell->exit_code = 1;
		return ;
	}
	if (chdir(home) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: %s\n", home, strerror(errno));
		mshell->exit_code = 1;
		return ;
	}
	update_pwd(mshell);
}

static void	cd_to_oldpwd(t_shell *mshell)
{
	const char	*oldpwd;

	oldpwd = env_find_value(mshell, "OLDPWD");
	if (!oldpwd)
	{
		ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
		mshell->exit_code = 1;
		return ;
	}
	if (chdir(oldpwd) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", oldpwd);
		perror("");
		mshell->exit_code = 1;
		return ;
	}
	ft_printf_fd(1, "%s\n", oldpwd);
	update_pwd(mshell);
}

static int	pwd_manual(t_shell *mshell, const char *fallback_path)
{
	char	*temp_pwd;
	char	*new_pwd;

	ft_printf_fd(2, "minishell: cd: error retrieving current directory: "
		"getcwd: cannot access parent directories: %s\n", strerror(errno));
	temp_pwd = ft_strjoin("/", fallback_path);
	if (!temp_pwd)
		return (1);
	new_pwd = ft_strjoin(mshell->curr_pwd, temp_pwd);
	free(temp_pwd);
	if (!new_pwd)
		return (1);
	free(mshell->old_pwd);
	mshell->old_pwd = ft_strdup(mshell->curr_pwd);
	free(mshell->curr_pwd);
	mshell->curr_pwd = ft_strdup(new_pwd);
	free(new_pwd);
	if (!mshell->old_pwd || !mshell->curr_pwd)
		return (1);
	if (env_find_value(mshell, "PWD"))
		env_add(mshell, "OLDPWD", env_find_value(mshell, "PWD"));
	env_add(mshell, "PWD", mshell->curr_pwd);
	// //true behavior
	// if (env_find_value(mshell, "PWD"))
	// 	env_add(mshell, "PWD", mshell->curr_pwd);
	// if (env_find_value(mshell, "OLDPWD"))
	// 	env_add(mshell, "OLDPWD", mshell->old_pwd);
	mshell->exit_code = 0;
	return (0);
}

static void	cd_to_path(t_shell *mshell, char *path)
{
	char *cwd;

	if (chdir(path) != 0)
	{
		ft_printf_fd(2, "minishell: cd: %s: %s\n", path, strerror(errno));
		mshell->exit_code = 1;
		return;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (pwd_manual(mshell, path))
		{
			mshell->exit_code = 1;
			return;
		}
	}
	else
	{
		free(cwd);
		update_pwd(mshell);
	}
}


int	builtin_cd(t_shell *mshell, char **token)
{
	if (!mshell || !token)
	{
		mshell->exit_code = 1;
		return (mshell->exit_code);
	}
	if (token[1] && token[2])
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		mshell->exit_code = 1;
		return (mshell->exit_code);
	}
	if (!token[1] || token[1][0] == '\0' || ft_strcmp(token[1], "--") == 0)
		cd_to_home(mshell);
	else if (ft_strcmp(token[1], "-") == 0)
		cd_to_oldpwd(mshell);
	else
		cd_to_path(mshell, token[1]);
	env_add(mshell, "_", "cd");
	return (mshell->exit_code);
}
