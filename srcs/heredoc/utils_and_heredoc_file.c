/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_and_heredoc_file.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:06:06 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/13 14:23:33 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static char	*make_heredoc_filename(int id)
{
	const char	*prefix;
	char		*id_str;
	char		*path;
	int			i;
	int			j;

	prefix = "/tmp/.heredoc_";
	i = 0;
	j = 0;
	id_str = ft_itoa(id);
	if (!id_str)
		return (NULL);
	path = malloc(ft_strlen(prefix) + ft_strlen(id_str) + 1);
	if (!path)
		return (free(id_str), NULL);
	while (prefix[i])
	{
		path[i] = prefix[i];
		i++;
	}
	while (id_str[j])
		path[i++] = id_str[j++];
	path[i] = '\0';
	free(id_str);
	return (path);
}

static int	assign_heredoc_path(t_shell *mshell, t_redirect *redir, char *name,
		char **path)
{
	redir->tmp_file = ft_strdup(name);
	free(name);
	if (!redir->tmp_file)
		return (error_msg(mshell, "heredoc: memory allocation failed", 0));
	*path = redir->tmp_file;
	return (0);
}

int	create_tmp_file(t_shell *mshell, t_redirect *redir, char **path, int *fd)
{
	char	*name;
	int		attempt;

	attempt = 0;
	while (attempt < 10000) 
	{
		name = make_heredoc_filename(mshell->heredoc_index + attempt);
		if (!name)
			return (error_msg(mshell, "heredoc: memory allocation failed", 0));
		*fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (*fd != -1)
		{
			mshell->heredoc_index += attempt + 1;
			return (assign_heredoc_path(mshell, redir, name, path));
		}
		free(name);
		if (errno != EEXIST)
			return (error_msg(mshell, "heredoc: open", 1));
		++attempt;
	}
	return (error_msg(mshell, "cannot create heredoc file", 0));
}

int	cleanup_heredoc_failure(t_redirect *redir, int fd, const char *path)
{
	close(fd);
	unlink(path);
	if (redir->tmp_file)
	{
		free(redir->tmp_file);
		redir->tmp_file = NULL;
	}
	return (1);
}
