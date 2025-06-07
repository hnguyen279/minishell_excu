#include "../../includes/shell.h"

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

static int	create_tmp_file(t_shell *mshell, t_redirect *redir, char **path,
		int *fd)
{
	char	*name;
	int		attempt;

	attempt = 0;
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

static int	prepare_delimiter(t_shell *mshell, t_redirect *redir, char **delim,
		int *expand)
{
	*delim = get_delimiter(redir->file);
	if (!*delim)
		return (error_msg(mshell, "heredoc: memory allocation failed", 0));
	*expand = !is_fully_quoted(redir->file);
	return (0);
}

// static int	process_heredoc_line(t_shell *mshell, int fd, char *line,
// 		int expand)
// {
// 	int	status;

// 	status = 0;
// 	if (expand)
// 		exe_handle_dollar_expansion(line, fd, mshell);
// 	else if (write(fd, line, strlen(line)) == -1)
// 		status = error_msg(mshell, "heredoc: write", 1);
// 	if (!status && write(fd, "\n", 1) == -1)
// 		status = error_msg(mshell, "heredoc: write", 1);
// 	free(line);
// 	return (status);
// }

// static int	check_heredoc_line_status(t_shell *mshell, char *line,
// 		const char *delim)
// {
// 	char	*expanded_line;

// 	if (g_signum == SIGINT)
// 	{
// 		free(line);
// 		return (-1);
// 	}
// 	if (!line)
// 	{
// 		ft_printf_fd(STDERR_FILENO,
// 			"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
// 			delim);
// 		return (1);
// 	}
// 	expanded_line = expand_token_value(line, mshell);
// 	if (ft_strcmp(expanded_line, delim) == 0)
// 	{
// 		free(expanded_line);
// 		free(line);
// 		return (2);
// 	}
// 	free(expanded_line);
// 	return (0);
// }

// static int	handle_heredoc_loop(t_shell *mshell, int fd, const char *delim,
// 		int expand)
// {
// 	char	*line;
// 	int		status;
// 	int		check;

// 	status = 0;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		check = check_heredoc_line_status(mshell, line, delim);
// 		if (check == -1)
// 			return (-1);
// 		else if (check == 1)
// 			break ;
// 		else if (check == 2)
// 			break ;
// 		status = process_heredoc_line(mshell, fd, line, expand);
// 		if (status)
// 			break ;
// 	}
// 	return (status);
// }



// static char *expand_heredoc_line(char *line, t_shell *mshell, int expand)
// {
// 	if (expand)
// 		return expand_token_value(line, mshell);
// 	return ft_strdup(line);
// }

// static int check_heredoc_line_status(char *line, const char *delim, char *expanded_line)
// {
// 	if (ft_strcmp(expanded_line, delim) == 0)
// 	{
// 		// free(line);
// 		free(expanded_line);
// 		return 2;
// 	}
// 	free(line);
// 	return 0;
// }

// static int process_heredoc_line(t_shell *mshell, int fd, char *line)
// {
// 	int status = 0;

// 	if (write(fd, line, strlen(line)) == -1)
// 		status = error_msg(mshell, "heredoc: write", 1);
// 	if (!status && write(fd, "\n", 1) == -1)
// 		status = error_msg(mshell, "heredoc: write", 1);
// 	free(line);
// 	return status;
// }

// static int handle_heredoc_loop(t_shell *mshell, int fd, const char *delim,
// 		int expand)
// {
// 	char *line;
// 	char *expanded_line;
// 	int status = 0;
// 	int check;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (g_signum == SIGINT)
// 		{
// 			if(line)
// 				free(line);
// 			return -1;
// 		}
// 		if (!line)
// 		{
// 			ft_printf_fd(STDERR_FILENO,
// 				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
// 				delim);
// 			break;
// 		}
// 		expanded_line = expand_heredoc_line(line, mshell, expand);
// 		if (!expanded_line)
// 		{
// 			free(line);
// 			return error_msg(mshell, "heredoc: expansion failed", 0);
// 		}
// 		check = check_heredoc_line_status(line, delim, expanded_line);
// 		if (check == -1)
// 			return -1;
// 		if (check == 1 || check == 2)
// 			break;
// 		status = process_heredoc_line(mshell, fd, expanded_line);
// 		if (status)
// 			break;
// 	}
// 	return status;
// }

//do to divide later
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
			return -1;
		}
		if (!line)
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				delim);
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
			status = error_msg(mshell, "heredoc: write", 1);
			free(expanded);
			break;
		}
		free(expanded);
	}
	return status;
}

static int	write_heredoc(t_shell *mshell, int fd, const char *delim,
		int expand)
{
	int	status;
	//int	tty_fd;

	g_signum = 0;
	status = handle_heredoc_loop(mshell, fd, delim, expand);
	free((void *)delim);
	if (status == -1)
	{
		sig_exit_code(mshell);
		// tty_fd = open("/dev/tty", O_RDONLY);
		// if (tty_fd == -1)
		// {
		// 	error_msg(mshell, "open(/dev/tty)", 1);
		// 	return (-1);
		// }
		// if (dup2(tty_fd, STDIN_FILENO) == -1)
		// {
		// 	error_msg(mshell, "dup2", 1);
		// 	close(tty_fd);
		// 	return (-1);
		// }
		// close(tty_fd);
	}
	setup_signals(mshell, MODE_HEREDOC);
	return (status);
}

static int	cleanup_heredoc_failure(t_redirect *redir, int fd, const char *path)
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

int	open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
{
	char	*path;
	char	*delim;
	int		fd;
	int		expand;
	int		status;

	if (!redir || !redir->file)
		return (error_msg(mshell, "heredoc: no delimiter", 0));
	if (create_tmp_file(mshell, redir, &path, &fd))
		return (1);
	if (prepare_delimiter(mshell, redir, &delim, &expand))
		return (cleanup_heredoc_failure(redir, fd, path));
	status = write_heredoc(mshell, fd, delim, expand);
	if (status != 0)
		return (cleanup_heredoc_failure(redir, fd, path));
	close(fd);
	mshell->exit_code = 0;
	return (0);
}

// int	open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
// {
// 	char	*path;
// 	char	*delim;
// 	int		fd;
// 	int		expand;

// 	if (!redir->file)
// 		return (error_msg(mshell, "heredoc: no delimiter", 0));
// 	if (create_tmp_file(mshell, redir, &path, &fd))
// 		return (1);
// 	if (prepare_delimiter(mshell, redir, &delim, &expand))
// 		return (cleanup_heredoc_failure(redir, fd, path));
// 	if (write_heredoc(mshell, fd, delim, expand))
// 		return cleanup_heredoc_failure(redir, fd, path);
// 	close(fd);
// 	mshell->exit_code = 0;
// 	return (0);
// }
