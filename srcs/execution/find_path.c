
#include "../../includes/shell.h"

static char **find_path(char **env)
{
	int		i = 0;
	char	**paths;

	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i] || env[i][5] == '\0')
	{
		paths = (char **)malloc(sizeof(char *) * 2);
		if (!paths)
			return (NULL);
		paths[0] = getcwd(NULL, 0);
		if (!paths[0])
		{
			free_split(paths);
			return (NULL);
		}
		paths[1] = NULL;
		return (paths);
	}
	return (ft_split(env[i] + 5, ':'));
}

static char	*build_cmd_path(char *first_cmd, char **env_paths)
{
	char	*one_path;
	char	*cmd_path;
	int		i = 0;

	while (env_paths[i])
	{
		one_path = ft_strjoin(env_paths[i], "/");
		// printf("one path in %s\n", one_path);
		if (!one_path)
		{
			free_split(env_paths);
			return (NULL);
		}
		cmd_path = ft_strjoin(one_path, first_cmd);
		// printf("cmd path in %s\n", cmd_path);
		free(one_path);
		if (!cmd_path)
		{
			print_error("no cmd path from build cmd path");
			free_split(env_paths);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_split(env_paths);
			// printf("cmd path in %s\n", cmd_path);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_split(env_paths);
	return (NULL);
}

static char *get_path(char *first_cmd, char **env)
{
	char	**paths = find_path(env);
	char	*cmd_path;

	if (!paths)
		return (NULL);
	cmd_path = build_cmd_path(first_cmd, paths);
	if (!cmd_path)
	{
		//print_error("no cmd_path from get_path function");
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
			ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n", cmd);
			mshell->exit_code = 126;
			return (126);
		}
		else if (access(cmd, F_OK) != 0)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: %s: No such file or directory\n", cmd);
			mshell->exit_code = 127;
			return (127);
		}
	}
	return (EXIT_SUCCESS);
}

char *find_cmd_path(t_shell *mshell, char *cmd)
{
    char *cmd_path;

    if (!cmd || !cmd[0] || !ft_strcmp(cmd, ".") || !ft_strcmp(cmd, ".."))
    {
        mshell->exit_code = display_error_cmd(cmd);
        return (NULL);
    }
    if (check_is_directory(mshell, cmd) != EXIT_SUCCESS)
        return (NULL);
    if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
        return (ft_strdup(cmd));
    cmd_path = get_path(cmd, mshell->envp);
    if (!cmd_path)
    {
        ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
        mshell->exit_code = 127;
        return (NULL);
    }
    if (access(cmd_path, X_OK) != 0)
    {
        ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n", cmd_path);
        mshell->exit_code = 126;
        free(cmd_path);
        return (NULL);
    }
    return (cmd_path);
}
