/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:16 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/18 01:58:37 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	run_command_child(t_ast *node, t_shell *mshell, char *cmd_path)
{
	setup_signals(mshell, MODE_CHILD);
	if (execve(cmd_path, node->cmd, mshell->envp) == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", node->cmd[0], strerror(errno));
		free(cmd_path);
		shell_cleanup(mshell);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}

static int	fork_and_exec(t_ast *node, t_shell *mshell, char *cmd_path)
{
	pid_t	pid;
	int		status;

	// printf("fork exec \n"); //debug
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		ft_printf_fd(2, "minishell: fork: %s\n", strerror(errno));
		mshell->exit_code = 1;
		return (1);
	}
	if (pid == 0)
		run_command_child(node, mshell, cmd_path);
	free(cmd_path);
	return (wait_command(mshell, pid, &status, 1));
}

static int	restore_stdio(t_shell *mshell, int in_fd, int out_fd)
{
	if (in_fd < 0 || out_fd < 0)
		return error_msg(mshell, "restore_stdio: invalid fd", 0);
	if ((dup2(in_fd, STDIN_FILENO) == -1) || (dup2(out_fd, STDOUT_FILENO) == -1))
	{
		safe_close_fds(in_fd, out_fd);
		return error_msg(mshell, "dup2 restore failed", 1);
	}
	safe_close_fds(in_fd, out_fd);
	return 0;
}

static int	prepare_redirection(t_shell *mshell, t_ast *node, int *in_fd, int *out_fd)
{
	*in_fd = dup(STDIN_FILENO);
	*out_fd = dup(STDOUT_FILENO);
	if (*in_fd == -1 || *out_fd == -1)
	{
		safe_close_fds(*in_fd, *out_fd);
		return error_msg(mshell, "dup failed", 1);
	}
	if (exe_redirection(node->redirects, mshell) != 0)
	{
		safe_close_fds(*in_fd, *out_fd);
		return (1); 
	}
	return 0;
}

static void	execute_with_cmd(t_shell *mshell, t_ast *node)
{
	char	*cmd_path;

	env_set_last_argument(mshell, node->cmd);
	if (is_builtin(node->cmd[0]))
		mshell->exit_code = execute_builtin(mshell, node->cmd);
	else
	{
		cmd_path = find_cmd_path(mshell, node->cmd[0]);
		if (!cmd_path)
			return ;
		mshell->exit_code = fork_and_exec(node, mshell, cmd_path);
	}
}

int	execute_command(t_ast *node, t_shell *mshell)
{
	int		in_fd;
	int		out_fd;

	in_fd = -1;
	out_fd = -1;
	if (node->redirects)
	{
		if (prepare_redirection(mshell, node, &in_fd, &out_fd) != 0)
			return mshell->exit_code;
	}
	if ((!node->cmd || !node->cmd[0]) && node->redirects)
	{
		if (restore_stdio(mshell, in_fd, out_fd) != 0)
			return 1;
		mshell->exit_code = 0;
		return mshell->exit_code;
	}
	execute_with_cmd(mshell, node);
	if (node->redirects)
	{
		if (restore_stdio(mshell, in_fd, out_fd) != 0)
			return 1;
	}
	return mshell->exit_code;
}
