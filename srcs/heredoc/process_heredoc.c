#include "../../includes/shell.h"

char	*make_heredoc_filename(int id)
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
