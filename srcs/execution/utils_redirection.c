/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:17:13 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/15 14:55:46 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	redirect_input(t_redirect *redir)
{
	int	fd;

	if (!redir || !redir->file)
		return (1);
	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_output(t_redirect *redir)
{
	int	fd;

	if (!redir || !redir->file)
		return (1);
	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_output_append(t_redirect *redir)
{
	int	fd;

	if (!redir || !redir->file)
		return (1);
	fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", redir->file, strerror(errno));
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	redirect_heredoc(t_redirect *redir, t_shell *mshell)
{
	int	fd;

	if (!redir || !redir->tmp_file)
		return (error_msg(mshell, "heredoc: no temporary file", 0));
	fd = open(redir->tmp_file, O_RDONLY);
	if (fd == -1)
	{
		ft_printf_fd(2, "minishell: %s: %s\n", redir->tmp_file,
			strerror(errno));
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_single_redirection(t_shell *mshell, t_redirect *redir)
{
	if (is_ambiguous_redirect(mshell, redir) != 0)
	{
		ft_printf_fd(2, "minishell: ambiguous redirect: %s\n", redir->file);
		return (1);
	}
	if (redir->type == REDIR_IN)
		return (redirect_input(redir));
	else if (redir->type == REDIR_OUT)
		return (redirect_output(redir));
	else if (redir->type == REDIR_APPEND)
		return (redirect_output_append(redir));
	else if (redir->type == REDIR_HEREDOC)
		return (redirect_heredoc(redir, mshell));
	else
	{
		ft_printf_fd(2, "minishell: invalid redirection type: %d\n",
			redir->type);
		return (1);
	}
}
