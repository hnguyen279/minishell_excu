/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:32 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:40:19 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	exe_redirection(t_redirect *redir, t_shell *mshell)
{
	t_redirect	*current;
	int			result;

	current = redir;
	if (!mshell)
	{
		ft_printf_fd(2, "minishell: internal error: null shell pointer\n");
		return (1);
	}
	while (current)
	{
		result = handle_single_redirection(mshell, current);
		if (result != 0)
		{
			mshell->exit_code = result;
			return (result);
		}
		current = current->next;
	}
	mshell->exit_code = 0;
	return (0);
}

void	safe_close_fds(int in_fd, int out_fd)
{
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
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
