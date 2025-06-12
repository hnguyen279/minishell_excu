/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:11:40 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/13 00:53:41 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**find_path(char **env)
{
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i] || env[i][5] == '\0')
		return (NULL);
	return (ft_split(env[i] + 5, ':'));
}

int	is_ambiguous_redirect(t_shell *mshell, t_redirect *redir)
{
	if (!redir || !redir->file || !redir->ori_file)
		return (0);
	if (redir->file[0] == '\0' && redir->ori_file[0] != '\0'
		&& ft_strchr(redir->ori_file, '$') && !is_fully_quoted(redir->ori_file)
		&& redir->type != REDIR_HEREDOC)
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: `%s`\n",
			redir->ori_file);
		mshell->exit_code = 1;
		return (1);
	}
	if (ft_strchr(redir->file, ' ') && ft_strchr(redir->ori_file, '$')
		&& !is_fully_quoted(redir->ori_file)
		&& !is_white_spaces_cmd(redir->file) && redir->type != REDIR_HEREDOC)
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: `%s`\n",
			redir->ori_file);
		mshell->exit_code = 1;
		return (1);
	}
	return (0);
}

int	wait_command(t_shell *mshell, pid_t pid, int *status, int update_exit_code)
{
	int	sig;

	if (waitpid(pid, status, 0) == -1)
		return (error_msg(mshell, "waitpid", 1));
	if (!update_exit_code)
		return (0);
	if (WIFEXITED(*status))
		mshell->exit_code = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		mshell->exit_code = 128 + sig;
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else
		mshell->exit_code = 1;
	return (mshell->exit_code);
}

int	display_error_cmd(char *cmd)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '\0')
	{
		ft_printf_fd(STDERR_FILENO, "minishell: Command '' not found\n");
		return (127);
	}
	else if (!ft_strcmp(cmd, "."))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: .: filename argument required\n"
			".: usage: . filename [arguments]\n");
		return (2);
	}
	else if (!ft_strcmp(cmd, ".."))
	{
		ft_printf_fd(STDERR_FILENO, "minishell: ..: command not found\n");
		return (127);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "minishell: %s: command not found\n", cmd);
		return (127);
	}
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
