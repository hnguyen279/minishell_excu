/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:28:47 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/15 10:42:52 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	check_heredoc_line(char *line, const char *delim)
{
	if (g_signum == SIGINT)
	{
		free(line);
		return (-1);
	}
	if (!line)
	{
		ft_printf_fd(2, "minishell: "
			"warning: here-document delimited by end-of-file (wanted `%s')\n",
			delim);
		return (1);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static char	*get_expanded_line(char *line, int expand, t_shell *mshell)
{
	char	*expanded;

	if (expand)
		expanded = expand_token_value(line, mshell);
	else
		expanded = ft_strdup(line);
	free(line);
	return (expanded);
}

static int	write_heredoc_line(t_shell *mshell, int fd, char *expanded)
{
	if (!expanded)
		return (error_msg(mshell, "heredoc: expansion failed", 0));
	if ((write(fd, expanded, ft_strlen(expanded)) == -1) || (write(fd, "\n",
				1) == -1))
	{
		free(expanded);
		return (error_msg(mshell, "heredoc: write", 1));
	}
	free(expanded);
	return (0);
}

static int	handle_heredoc_loop(t_shell *mshell, int fd, const char *delim,
		int expand)
{
	char	*line;
	char	*expanded;
	int		status;
	int		check;

	status = 0;
	while (1)
	{
		line = readline("> ");
		check = check_heredoc_line(line, delim);
		if (check == -1)
			return (-1);
		if (check == 1)
			break ;
		expanded = get_expanded_line(line, expand, mshell);
		if (write_heredoc_line(mshell, fd, expanded) != 0)
		{
			status = 1;
			break ;
		}
	}
	return (status);
}

int	write_heredoc(t_shell *mshell, int fd, const char *delim, int expand)
{
	int	status;

	g_signum = 0;
	status = handle_heredoc_loop(mshell, fd, delim, expand);
	free((void *)delim);
	if (status == -1)
		sig_exit_code(mshell);
	setup_signals(mshell, MODE_HEREDOC);
	return (status);
}
