/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:40 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/13 23:07:47 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static char	*try_path_combination(char *path_prefix, char *first_cmd)
{
	char	*one_path;
	char	*cmd_path;

	one_path = ft_strjoin(path_prefix, "/");
	if (!one_path)
		return (NULL);
	cmd_path = ft_strjoin(one_path, first_cmd);
	free(one_path);
	if (!cmd_path)
		return (NULL);
	if (access(cmd_path, F_OK | X_OK) == 0)
		return (cmd_path);
	free(cmd_path);
	return (NULL);
}

static char	*build_cmd_path(char *first_cmd, char **env_paths)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (env_paths[i])
	{
		cmd_path = try_path_combination(env_paths[i], first_cmd);
		if (cmd_path)
		{
			free_split(env_paths);
			return (cmd_path);
		}
		i++;
	}
	free_split(env_paths);
	return (NULL);
}

static char	*get_path(char *first_cmd, char **env)
{
	char	**paths;
	char	*cmd_path;

	paths = find_path(env);
	if (!paths)
		return (NULL);
	cmd_path = build_cmd_path(first_cmd, paths);
	if (!cmd_path)
	{
		return (NULL);
	}
	return (cmd_path);
}

static int	check_is_directory(t_shell *mshell, char *cmd)
{
	struct stat	statbuf;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: Is a directory\n", cmd);
			mshell->exit_code = 126;
			return (126);
		}
		else if (stat(cmd, &statbuf) == 0 && access(cmd, X_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n",
				cmd);
			mshell->exit_code = 126;
			return (126);
		}
		else if (access(cmd, F_OK) != 0)
		{
			ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd);
			mshell->exit_code = 127;
			return (127);
		}
	}
	return (EXIT_SUCCESS);
}

char	*find_cmd_path(t_shell *mshell, char *cmd)
{
	char	*cmd_path;

	//debug
	//printf("cmd = [%s]\n", cmd); //
	if (!cmd || cmd[0] == '\0' || !ft_strcmp(cmd, ".") || !ft_strcmp(cmd, ".."))
		return (mshell->exit_code = display_error_cmd(cmd), NULL);
	if (check_is_directory(mshell, cmd) != EXIT_SUCCESS)
		return (NULL);
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	cmd_path = get_path(cmd, mshell->envp);
	if (!cmd_path)
		return (handle_path_error(mshell, cmd, 127));
	if (access(cmd_path, X_OK) != 0)
	{
		// free(cmd_path); // recheck, cmd_path is used after free
		return (handle_path_error(mshell, cmd_path, 126));
	}
	return (cmd_path);
}
