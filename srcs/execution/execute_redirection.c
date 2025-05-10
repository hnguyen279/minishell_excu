
#include "minishell.h"

static int redirect_input(t_redirect *redir)
{
    redir->fd = open(redir->file, O_RDONLY);
    if (redir->fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                     redir->file, strerror(errno));
        return (1);
    }
    if (dup2(redir->fd, STDIN_FILENO) == -1)
    {
        close(redir->fd);
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        return (1);
    }
    close(redir->fd);
    return (0);
}

static int redirect_output(t_redirect *redir)
{
    redir->fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (redir->fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
            redir->file, strerror(errno));
        return (1);
    }
    if (dup2(redir->fd, STDOUT_FILENO) == -1) 
    {
        close(redir->fd);
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        return (1);
    }
    close(redir->fd);
    return (0);
}

static int redirect_output_append(t_redirect *redir)
{
    redir->fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (redir->fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                     redir->file, strerror(errno));
        return (1);
    }
    if (dup2(redir->fd, STDOUT_FILENO) == -1)
    {
        close(redir->fd);
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        return (1);
    }
    close(redir->fd);
    return (0);
}

static int redirect_heredoc(t_redirect *redir, t_shell *mshell)
{
    if (redir->fd < 0)
    {
        ft_printf_fd(2, "minishell: heredoc was not processed\n");
        mshell->exit_code = 1;
        return (1);
    }
    if (dup2(redir->fd, STDIN_FILENO) == -1)
    {
        close(redir->fd);
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        mshell->exit_code = 1;
        return (1);
    }
    close(redir->fd);
    if (redir->tmp_file)
    {
        unlink(redir->tmp_file);
        free(redir->tmp_file);
        redir->tmp_file = NULL;
    }
    mshell->exit_code = 0;
    return (0);
}

int exe_redirection(t_redirect *redir, t_shell *mshell)
{
    while (redir)
    {
       
        if (check_ambigous_redir(mshell, redir, &redir->fd) != 0)
            return (mshell->exit_code);
        if (redir->type == REDIR_IN)
            mshell->exit_code = redirect_input(redir);
        else if (redir->type == REDIR_OUT)
            mshell->exit_code = redirect_output(redir);
        else if (redir->type == REDIR_APPEND)
            mshell->exit_code = redirect_output_append(redir);
        else if (redir->type == REDIR_HEREDOC)
            mshell->exit_code = redirect_heredoc(redir);
        else
        {
            ft_printf_fd(2, "minishell: invalid redirection type\n"); // NOt sure
            mshell->exit_code = 1;
        }
        if (mshell->exit_code != 0)
            return (mshell->exit_code);
        redir = redir->next;
    }
    return (0);
}
