/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:24 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/14 06:15:55 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	setup_pipe_redirection(int pipe_fd_src, int std_fd_dst)
{
	if (dup2(pipe_fd_src, std_fd_dst) == -1)
	{
		ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
		close(pipe_fd_src);
		exit(1);
	}
	close(pipe_fd_src);
}

static void	execute_child(t_shell *mshell, t_ast *node, int *pipe_fd, int left)
{
	t_ast	*child;

	setup_signals(mshell, MODE_CHILD);
	if (!node)
		exit(1);
	if (left)
		child = node->left;
	else
		child = node->right;
	if (!child)
		exit(1);
	if (child->redirects && exe_redirection(child->redirects, mshell) != 0)
	{
				
				shell_cleanup(mshell);




				exit(mshell->exit_code);

	}
	if (left)
	{
		close(pipe_fd[FD_READ]);
		setup_pipe_redirection(pipe_fd[FD_WRITE], STDOUT_FILENO);
	}
	else
	{
		close(pipe_fd[FD_WRITE]);
		setup_pipe_redirection(pipe_fd[FD_READ], STDIN_FILENO);
	}
	execute_ast(child, mshell);

	shell_cleanup(mshell);


	exit(mshell->exit_code);
}

static int	init_child(int *pipe_fd, pid_t *pid, t_shell *mshell)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("minishell: fork");
		close(pipe_fd[FD_READ]);
		close(pipe_fd[FD_WRITE]);
		mshell->exit_code = 1;
		return (-1);
	}
	return (0);
}

int	execute_pipe(t_ast *node, t_shell *mshell)
{
	int		pipe_fd[2];
	pid_t	pid[2];
	int		status[2];

	if (!node || !node->left || !node->right)
		return (error_msg(mshell, "syntax error near unexpected token `|'", 0));
	if (pipe(pipe_fd) == -1)
		return (error_msg(mshell, "pipe", 1));
	if (init_child(pipe_fd, &pid[0], mshell) == -1)
		return (mshell->exit_code);
	if (pid[0] == 0)
		execute_child(mshell, node, pipe_fd, 1);
	if (init_child(pipe_fd, &pid[1], mshell) == -1)
		return (mshell->exit_code);
	if (pid[1] == 0)
		execute_child(mshell, node, pipe_fd, 0);
	close(pipe_fd[FD_READ]);
	close(pipe_fd[FD_WRITE]);
	wait_command(mshell, pid[0], &status[0], 0);
	wait_command(mshell, pid[1], &status[1], 1);
	return (mshell->exit_code);
}
