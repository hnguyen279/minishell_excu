
#include "../../includes/shell.h"

static int redirect_input(t_redirect *redir)
{
    if (!redir || !redir->file)
        return (1);
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
    if (!redir || !redir->file)
        return (1);
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
    if (!redir || !redir->file)
        return (1);
    int fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
                    redir->file, strerror(errno));
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        printf("dup2 failed\n");
        close(fd);
        // ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        return (1);
    }
    close(fd);
    return (0);
}

static int redirect_heredoc(t_redirect *redir, t_shell *mshell)
{
    int fd;

    if (!redir || !redir->tmp_file)
        return display_error_errno(mshell, "heredoc: no temporary file", 0);
    fd = open(redir->tmp_file, O_RDONLY);
    //printf("tmp_heredoc_name: %s \n", redir->tmp_file);
    if (fd == -1)
        return display_error_errno(mshell, redir->tmp_file, 1);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return display_error_errno(mshell, "dup2", 1);
    }
    close(fd);
    return (0);
}

static int handle_single_redirection(t_shell *mshell, t_redirect *redir)
{
    if (is_ambiguous_redirect(mshell, redir) != 0)
    {
        // printf("ambiguous redirect");
        mshell->exit_code = 1;
        return mshell->exit_code;
    }
    if (redir->type == REDIR_IN)
        mshell->exit_code = redirect_input(redir);
    else if (redir->type == REDIR_OUT)
        mshell->exit_code = redirect_output(redir);
    else if (redir->type == REDIR_APPEND)
    {
        // printf("APPEND redirect\n");
        mshell->exit_code = redirect_output_append(redir);
    }
    else if (redir->type == REDIR_HEREDOC)
        mshell->exit_code = redirect_heredoc(redir, mshell);
    else
    {
        ft_printf_fd(2, "minishell: invalid redirection type: %d\n", redir->type);
        mshell->exit_code = 1;
    }
    return (mshell->exit_code);
}

int exe_redirection(t_redirect *redir, t_shell *mshell)
{
    t_redirect *current;
    int result;

    current = redir;
    if (!mshell)
    {
        ft_printf_fd(2, "minishell: internal error: null shell pointer\n");
        return 1;
    }
    // printf("exe_redirection worked\n");
    while (current)
    {
        result = handle_single_redirection(mshell, current);
        if (result != 0)
        {
            mshell->exit_code = result;
            return (result);
        }
        current = current->next;
    }
    mshell->exit_code = 0;
    return (0);
}
