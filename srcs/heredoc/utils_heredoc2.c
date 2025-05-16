#include "../../includes/shell.h"

static int	ft_error(t_shell *mshell, const char *msg)
{
	if (msg)
		//ft_printf_fd(2, "minishell: %s\n", msg);
	if (mshell)
		mshell->exit_code = 1;
	return (1); 
}

static int  validate_heredoc_redirect(t_shell *mshell, t_redirect *redir)
{
    if (!redir->file)
    {
        //ft_printf_fd(2, "minishell: heredoc: no delimiter\n");
        mshell->exit_code = 1;
        return (1);
    }
    return (0);
}
static int  create_tmp_file(t_shell *mshell, t_redirect *redir, char **path, int *fd)
{
    char    *name;
    int     attempt;

    attempt = 0;
    while (attempt < 1000)
    {
        name = make_heredoc_filename_from_fd(mshell->heredoc_index + attempt);
        if (!name)
            return (ft_error(mshell, "heredoc: memory allocation failed"));
        *fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
        if (*fd != -1)
        {
            mshell->heredoc_index += attempt + 1;
            redir->tmp_file = ft_strdup(name);
            free(name);
            if (!redir->tmp_file)
                return (ft_error(mshell, "heredoc: memory allocation failed"));
            *path = redir->tmp_file;
            return (0);
        }
        free(name);
        if (errno != EEXIST)
            return (ft_error(mshell, "heredoc: open"));
        ++attempt;
    }
    return (ft_error(mshell, "minishell: cannot create heredoc file"));
}

static int  prepare_delimiter(t_shell *mshell, t_redirect *redir,char **delim, int *expand)
{
    *delim = get_delimiter(redir->file);
    if (!*delim)
        return (ft_error(mshell, "heredoc: memory allocation failed"));
    *expand = !inside_quotes(redir->file);
    setup_signals(MODE_HEREDOC);
    return (0);
}
static int  process_heredoc_line(t_shell *mshell, int fd,char *line, int expand)
{
    int status;

    status = 0;
    if (expand)
        exe_handle_dollar_expansion(line, fd, mshell);
    else if (write(fd, line, strlen(line)) == -1)
        status = ft_error(mshell, "heredoc: write");
    if (!status && write(fd, "\n", 1) == -1)
        status = ft_error(mshell, "heredoc: write");
    free(line);
    return (status);
}

static int  write_heredoc(t_shell *mshell,int fd, const char *delim, int expand)
{
    char *line;
    int  status;
    
    status = 0;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            //ft_printf_fd(2,"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",delim);
            break;
        }
        if (!strcmp(line, delim))
        {
            free(line);
            break;
        }
        status = process_heredoc_line(mshell, fd, line, expand);
        if (status)
            break;
    }
    free((void *)delim);
    return (status);
}

int  open_heredoc_pipe(t_shell *mshell, t_redirect *redir)
{
    char    *path;
    char    *delim;
    int     fd;
    int     expand;

    if (validate_heredoc_redirect(mshell, redir))
        return (1);
    if (create_tmp_file(mshell, redir, &path, &fd))
        return (1);
    if (prepare_delimiter(mshell, redir, &delim, &expand))
        return (1);
    if (write_heredoc(mshell, fd, delim, expand))
    {
        close(fd);
        unlink(path);
        free(redir->tmp_file);
        return (1);
    }
    setup_signals(MODE_HEREDOC);
    close(fd);
    redir->fd = open(path, O_RDONLY);
    if (redir->fd < 0)
        return (ft_error(mshell, "heredoc: open"));
    mshell->exit_code = 0;
    return (0);
}
