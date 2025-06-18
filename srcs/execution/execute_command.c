/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:16 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/18 19:29:21 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"


static void	run_command_child(t_ast *node, t_shell *mshell, char *cmd_path)
{
	setup_signals(mshell, MODE_CHILD);
	if (node->redirects && exe_redirection(node->redirects, mshell) != 0)
	{
		free(cmd_path);
		shell_cleanup(mshell);
		exit(mshell->exit_code);
	}
	if (execve(cmd_path, node->cmd, mshell->envp) == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", node->cmd[0], strerror(errno));
		free(cmd_path);
		shell_cleanup(mshell);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126); // recheck
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
		//loop_clean(mshell);  // free ///check again?? > not need?
		ft_printf_fd(2, "minishell: fork: %s\n", strerror(errno));
		mshell->exit_code = 1;
		return (1);
	}
	if (pid == 0)
		run_command_child(node, mshell, cmd_path);
	free(cmd_path);
	return (wait_command(mshell, pid, &status, 1));
}

int	redirect_and_backup_fds(t_ast *node, t_shell *mshell, int *in_fd,
		int *out_fd)
{
	*in_fd = dup(STDIN_FILENO);
	*out_fd = dup(STDOUT_FILENO);
	if (*in_fd == -1 || *out_fd == -1)
	{
		safe_close_fds(*in_fd, *out_fd);
		return (error_msg(mshell, "dup failed", 1));
	}
	if (exe_redirection(node->redirects, mshell) != 0)
	{
		if ((dup2(*in_fd, STDIN_FILENO) == -1)
			|| (dup2(*out_fd, STDOUT_FILENO) == -1))
		{
			safe_close_fds(*in_fd, *out_fd);
			return (error_msg(mshell, "dup failed", 1));
		}
		safe_close_fds(*in_fd, *out_fd);
		return (mshell->exit_code);
	}
	return (0);
}
static int	execute_with_redirect(t_ast *node, t_shell *mshell, int is_builtin)
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (node->redirects && redirect_and_backup_fds(node, mshell, &in_fd, &out_fd) != 0)
	{
		//loop_clean(mshell);  // free ///check again ?????> not need?
		return (mshell->exit_code);
	}
	if (is_builtin)
		mshell->exit_code = execute_builtin(mshell, node->cmd);
	if (node->redirects)
	{
		if ((dup2(in_fd, 0) == -1) || (dup2(out_fd, 1) == -1))
		{
			safe_close_fds(in_fd, out_fd);
			//loop_clean(mshell); // free ///check again?????-> not need?
			return (error_msg(mshell, "dup failed", 1));
		}
		safe_close_fds(in_fd, out_fd);
	}
	return (mshell->exit_code);
}

int	execute_command(t_ast *node, t_shell *mshell)
{
	char	*cmd_path;

	// printf("execute command func \n"); //debug
	if (!node->cmd || !node->cmd[0])
	{
		if (node->redirects)
		{
			return (execute_with_redirect(node, mshell, 0));
		}
		mshell->exit_code = display_error_cmd(NULL);
		return (mshell->exit_code);
	}
	env_set_last_argument(mshell, node->cmd);
	if (is_builtin(node->cmd[0]))
	{
		return (execute_with_redirect(node, mshell, 1));
	}
	cmd_path = find_cmd_path(mshell, node->cmd[0]);
	// printf("find cmd_path %s\n", cmd_path);
	if (!cmd_path)
		return (mshell->exit_code);
	mshell->exit_code = fork_and_exec(node, mshell, cmd_path);
	return (mshell->exit_code);
}
