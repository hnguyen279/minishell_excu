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
	return (error_msg(mshell, "minishell: cannot create heredoc file", 0));
}

int	is_quoted(const char *str)
{
	size_t	len;

	if (!str || !str[0])
		return (0);
	len = ft_strlen(str);
	if ((str[0] == '\'' || str[0] == '"') && str[len - 1] == str[0])
	{
		if (len == 2)
			return 1;
		if (str[0] == '\'')
			return 2;
		else
			return 3;
	}
	else if ((str[0] == '\'' || str[0] == '"') ||
			 (str[len - 1] == '\'' || str[len - 1] == '"'))
		return -1;
	return 4;
}


char *get_delimiter(char *file)
{
	size_t len;

	if (!file)
		return NULL;

	len = strlen(file);

	if (len >= 2 &&
		(file[0] == '\'' || file[0] == '"') &&
		file[len - 1] == file[0])
	{
		return ft_substr(file, 1, len - 2); // bỏ quote đầu-cuối
	}

	return ft_strdup(file); // giữ nguyên
}


int prepare_delimiter(t_shell *mshell, t_redirect *redir, char **delim, int *expand)
{
	int quote_type;

	quote_type = is_quoted(redir->file);
	if (quote_type == -1)
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: heredoc: unmatched quote in delimiter: %s\n",
			redir->file);
		return error_msg(mshell, "heredoc", 1);
	}

	if (quote_type == 1 || quote_type == 2 || quote_type == 3)
	{
		*expand = 0;
		*delim = get_delimiter(redir->file);
	}
	else
	{
		*expand = 1;
		*delim = expand_token_value(redir->file, mshell);
		if (!*delim)
			*delim = ft_strdup(redir->file);
	}

	if (!*delim)
		return error_msg(mshell, "heredoc: memory allocation failed", 0);

	printf("prepare_delimiter: delim = [%s], expand = %d\n", *delim, *expand);
	return 0;
}

static int handle_heredoc_loop(t_shell *mshell, int fd, const char *delim, int expand)
{
	char *line;
	char *expanded;
	int status = 0;

	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
		{
			free(line);
			return (-1);
		}
		if (!line)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: warning: here-document delimited by end-of-file (wanted `%s`)\n", delim);
			break;
		}
		if (expand)
			expanded = expand_token_value(line, mshell);
		else
			expanded = ft_strdup(line);
		free(line);
		if (!expanded)
			return error_msg(mshell, "heredoc: expansion failed", 0);
		if (ft_strcmp(expanded, delim) == 0)
		{
			free(expanded);
			break;
		}
		if (write(fd, expanded, strlen(expanded)) == -1 || write(fd, "\n", 1) == -1)
		{
			error_msg(mshell, "heredoc: write", 1);
			status = 1;
			free(expanded);
			break;
		}
		free(expanded);
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

	if (!redir || !redir->file)
		return error_msg(mshell, "heredoc: no delimiter", 0);
	if (create_tmp_file(mshell, redir, &path, &fd))
		return 1;
	if (prepare_delimiter(mshell, redir, &delim, &expand))
		return cleanup_heredoc_failure(redir, fd, path);
	status = write_heredoc(mshell, fd, delim, expand);
	if (status != 0)
		return cleanup_heredoc_failure(redir, fd, path);
	close(fd);
	mshell->exit_code = 0;
	return 0;
}
