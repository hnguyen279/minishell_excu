
#include "../../includes/shell.h"

static int redirect_input(t_redirect *redir)
{
    int fd = open(redir->file, O_RDONLY);
    if (fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                    redir->file, strerror(errno));
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

static int redirect_output(t_redirect *redir)
{
    int fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                    redir->file, strerror(errno));
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

static int redirect_output_append(t_redirect *redir)
{
    int fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                    redir->file, strerror(errno));
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


static int redirect_heredoc(t_redirect *redir, t_shell *mshell)
{
    int fd;

    if (!redir->tmp_file)
    {
        ft_printf_fd(2, "minishell: heredoc: no temporary file\n");
        mshell->exit_code = 1;
        return (1);
    }
    fd = open(redir->tmp_file, O_RDONLY);
    if (fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                    redir->tmp_file, strerror(errno));
        mshell->exit_code = 1;
        return (1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        mshell->exit_code = 1;
        return (1);
    }
    close(fd);
    unlink(redir->tmp_file);
    free(redir->tmp_file);
    redir->tmp_file = NULL;
    return (0);
}

#include "../../includes/shell.h"

int exe_redirection(t_redirect *redir, t_shell *mshell)
{
    t_redirect *current = redir;

    if (!mshell)
    {
        ft_printf_fd(2, "minishell: internal error: null shell pointer\n");
        return (1);
    }
    while (current)
    {
        if (check_ambiguous_redir(mshell, current) != 0)
        {
            mshell->exit_code = 1;
            return (mshell->exit_code);
        }
        if (current->type == REDIR_IN)
            mshell->exit_code = redirect_input(current);
        else if (current->type == REDIR_OUT)
            mshell->exit_code = redirect_output(current);
        else if (current->type == REDIR_APPEND)
            mshell->exit_code = redirect_output_append(current);
        else if (current->type == REDIR_HEREDOC)
            mshell->exit_code = redirect_heredoc(current, mshell);
        else
        {
            ft_printf_fd(2, "minishell: invalid redirection type: %d\n", current->type);
            mshell->exit_code = 1;
        }
        if (mshell->exit_code != 0)
            return (mshell->exit_code);
        current = current->next;
    }
    return (0);
}
