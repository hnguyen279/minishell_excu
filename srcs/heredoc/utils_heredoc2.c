#include "../../includes/shell.h"

static int create_tmp_file(t_shell *mshell, t_redirect *redir, char **path, int *fd)
{
	char *name;
	int attempt;

	attempt = 0;
	while (attempt < 1000)
	{
		name = make_heredoc_filename(mshell->heredoc_index + attempt);
		if (!name)
			return display_error_errno(mshell, "heredoc: memory allocation failed", 0);
		*fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (*fd != -1)
		{
			mshell->heredoc_index += attempt + 1;
			redir->tmp_file = ft_strdup(name);
			free(name);
			if (!redir->tmp_file)
				return display_error_errno(mshell, "heredoc: memory allocation failed", 0);
			*path = redir->tmp_file;
			return (0);
		}
		free(name);
		if (errno != EEXIST)
			return display_error_errno(mshell, "heredoc: open", 1);
		++attempt;
	}
	return display_error_errno(mshell, "minishell: cannot create heredoc file", 0);
}

// static int create_tmp_file(t_shell *mshell, t_redirect *redir, char **path, int *fd)
// {
//     char *name;
//     int attempt = 0;

//     while (1)
//     {
//         name = make_heredoc_filename(mshell->heredoc_index + attempt);
//         if (!name)
//             return display_error_errno(mshell, "heredoc: malloc", 0);
//         *fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
//         if (*fd != -1)
//         {
//             mshell->heredoc_index += attempt + 1;
//             redir->tmp_file = ft_strdup(name);
//             free(name);
//             if (!redir->tmp_file)
//                 return display_error_errno(mshell, "heredoc: strdup", 0);
//             *path = redir->tmp_file;
//             return (0);
//         }
//         free(name);
//         if (errno != EEXIST)
//             return display_error_errno(mshell, "heredoc: open", 1);
//         ++attempt;
//     }
// }

static int prepare_delimiter(t_shell *mshell, t_redirect *redir, char **delim, int *expand)
{
	*delim = get_delimiter(redir->file);
	if (!*delim)
		return display_error_errno(mshell, "heredoc: memory allocation failed", 0);
	*expand = !is_fully_quoted(redir->file);
	return (0);
}

static int process_heredoc_line(t_shell *mshell, int fd, char *line, int expand)
{
	int status;

	status = 0;
	if (expand)
		exe_handle_dollar_expansion(line, fd, mshell);
	else if (write(fd, line, strlen(line)) == -1)
		status = display_error_errno(mshell, "heredoc: write", 1);
	if (!status && write(fd, "\n", 1) == -1)
		status = display_error_errno(mshell, "heredoc: write", 1);
	free(line);
	return status;
}

static int write_heredoc(t_shell *mshell, int fd, const char *delim, int expand)
{
    char *line;
    int status;

	status = 0;
    g_signum = 0; 
    while (1)
    {
        line = readline("> ");
        if (g_signum == SIGINT)
        {
            if (line)
                free(line);
            status = -1;
            break;
        }
        else if (!line)
        {
            ft_printf_fd(STDERR_FILENO,
                "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delim);
            break;
        }

        char *expanded_line = expand_token_value(line, mshell);
        if (ft_strcmp(expanded_line, delim) == 0)
        {
            free(expanded_line);
            free(line);
            break;
        }
        free(expanded_line);
        status = process_heredoc_line(mshell, fd, line, expand);
        if (status)
            break;
    }
    free((void *)delim);
    if (status == -1)
    {
        sig_exit_code(mshell);
        int tty_fd = open("/dev/tty", O_RDONLY);
		if (tty_fd == -1)
		{
			display_error_errno(mshell, "open(/dev/tty)", 1);
			return -1;
		}
		if (dup2(tty_fd, STDIN_FILENO) == -1)
		{
			display_error_errno(mshell, "dup2", 1);
			close(tty_fd);
			return -1;
		}
		close(tty_fd);
    }
    setup_signals(mshell, MODE_HEREDOC);
    return status;
}

int open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
{
	char *path;
	char *delim;
	int fd;
	int expand;

	if (!redir->file)
		return display_error_errno(mshell, "heredoc: no delimiter", 0);
	if (create_tmp_file(mshell, redir, &path, &fd))
		return 1;
	if (prepare_delimiter(mshell, redir, &delim, &expand))
	{
		close(fd);
		unlink(path);
		free(redir->tmp_file);
		redir->tmp_file = NULL;
		return 1;
	}
	if (write_heredoc(mshell, fd, delim, expand))
	{
		close(fd);
		unlink(path);
		free(redir->tmp_file);
		redir->tmp_file = NULL;
		return 1;
	}
	close(fd);
	mshell->exit_code = 0;
	return 0;
}


