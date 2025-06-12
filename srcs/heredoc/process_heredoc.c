/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:05:56 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:05:57 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
{
	char	*path;
	char	*delim;
	int		fd;
	int		expand;
	int		status;

	if (!redir || !redir->ori_file)
		return (error_msg(mshell, "heredoc: no delimiter", 0));
	if (create_tmp_file(mshell, redir, &path, &fd))
		return (1);
	if (prepare_delimiter(redir, &delim, &expand))
		return (cleanup_heredoc_failure(redir, fd, path));
	status = write_heredoc(mshell, fd, delim, expand);
	if (status != 0)
		return (cleanup_heredoc_failure(redir, fd, path));
	close(fd);
	mshell->exit_code = 0;
	return (0);
}

int	process_heredocs(t_shell *mshell, t_ast *node)
{
	t_redirect	*redir;

	if (!node)
		return (0);
	redir = node->redirects;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			if (open_heredoc_pipe(mshell, redir) != 0)
				return (mshell->exit_code);
		}
		redir = redir->next;
	}
	if (node->type == NODE_PIPE)
	{
		if (process_heredocs(mshell, node->left) != 0)
			return (mshell->exit_code);
		if (process_heredocs(mshell, node->right) != 0)
			return (mshell->exit_code);
	}
	return (0);
}
