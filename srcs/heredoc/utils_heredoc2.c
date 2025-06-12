#include "../../includes/shell.h"

static int assign_heredoc_path(t_shell *mshell, t_redirect *redir, char *name, char **path)
{
	redir->tmp_file = ft_strdup(name);
	free(name);
	if (!redir->tmp_file)
		return (error_msg(mshell, "heredoc: memory allocation failed", 0));
	*path = redir->tmp_file;
	return (0);
}

static int create_tmp_file(t_shell *mshell, t_redirect *redir, char **path, int *fd)
{
	char *name;
	int attempt = 0;

	while (attempt < 1000)
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


static void	process_quote_and_copy(const char *word, char *res, int *i, int *j)
{
	char	quote;

	quote = word[*i];
	(*i)++;
	while (word[*i] && word[*i] != quote)
		res[(*j)++] = word[(*i)++];
	if (word[*i] == quote)
		(*i)++;
}

char	*get_delimiter(char *word)
{
	int		i;
	int		j;
	char	*res;

	if (!word)
		return (NULL);
	res = malloc(ft_strlen(word) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '"')
			process_quote_and_copy(word, res, &i, &j);
		else
			res[j++] = word[i++];
	}
	res[j] = '\0';
	return (res);
}

int prepare_delimiter(t_redirect *redir, char **delim, int *expand)
{
	int quote_type;

	quote_type = is_fully_quoted(redir->ori_file);
	if (quote_type == -1)
	{
		ft_printf_fd(2, "minishell: heredoc: unmatched quote in delimiter: %s\n", redir->ori_file);
		return (1);
	}
	if (quote_type == 1 || quote_type == 2)
		*expand = 0;
	else
		*expand = 1;
	*delim = get_delimiter(redir->ori_file);
	if (!*delim)
	{
		ft_printf_fd(2, "minishell: heredoc: memory allocation failed\n");
		return (1);
	}
	//debug
	//printf("prepare_delimiter: delim = [%s], expand = %d\n", *delim, *expand);
	return 0;
}

static int	write_expanded_line(t_shell *mshell, int fd, char *expanded, char *eof)
{
	if (write(fd, expanded, strlen(expanded)) == -1 || write(fd, "\n", 1) == -1)
	{
		error_msg(mshell, "heredoc: write", 1);
		free(expanded);
		free(eof);
		return (1);
	}
	free(expanded);
	free(eof);
	return (0);
}

static int	process_line(t_shell *mshell, int fd, const char *delim, char *line, int expand)
{
	char *expanded;
	char *eof;

	eof = ft_strdup(line);
	if (!eof)
		return (error_msg(mshell, "heredoc: expansion failed", 0));
	if (expand)
		expanded = expand_token_value(line, mshell);
	else
		expanded = ft_strdup(line);
	if (!expanded)
	{
		free(eof);
		return (error_msg(mshell, "heredoc: expansion failed", 0));
	}
	if (ft_strcmp(eof, delim) == 0)
	{
		free(expanded);
		free(eof);
		return (2); // signal to break loop
	}
	return write_expanded_line(mshell, fd, expanded, eof);
}

static int	check_heredoc_line(char *line, const char *delim)
{
	if (g_signum == SIGINT)
	{
		free(line);
		return (-1); // aborted by Ctrl+C
	}
	if (!line)
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
			delim);
		return (1); // warning: EOF
	}
	return (0); // valid line
}

static int	handle_heredoc_loop(t_shell *mshell, int fd, const char *delim, int expand)
{
	char *line;
	int status;
	int result;
	int check;

	status = 0;
	while (1)
	{
		line = readline("> ");
		check = check_heredoc_line(line, delim);
		if (check == -1)
			return (-1);
		if (check == 1)
			break;
		result = process_line(mshell, fd, delim, line, expand);
		free(line);
		if (result == 2)
			break;
		if (result != 0)
		{
			status = 1;
			break;
		}
	}
	return (status);
}

static int write_heredoc(t_shell *mshell, int fd, const char *delim, int expand)
{
	int status;

	g_signum = 0;
	status = handle_heredoc_loop(mshell, fd, delim, expand);
	free((void *)delim);
	if (status == -1)
		sig_exit_code(mshell);
	setup_signals(mshell, MODE_HEREDOC);
	return status;
}

static int cleanup_heredoc_failure(t_redirect *redir, int fd, const char *path)
{
	close(fd);
	unlink(path);
	if (redir->tmp_file)
	{
		free(redir->tmp_file);
		redir->tmp_file = NULL;
	}
	return 1;
}

int open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
{
	char *path;
	char *delim;
	int fd;
	int expand;
	int status;

	if (!redir || !redir->ori_file)
		return error_msg(mshell, "heredoc: no delimiter", 0);
	if (create_tmp_file(mshell, redir, &path, &fd))
		return 1;
	if (prepare_delimiter(redir, &delim, &expand))
		return cleanup_heredoc_failure(redir, fd, path);
	status = write_heredoc(mshell, fd, delim, expand);
	if (status != 0)
		return cleanup_heredoc_failure(redir, fd, path);
	close(fd);
	mshell->exit_code = 0;
	return 0;
}
