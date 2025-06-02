
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
// debuging
// static int redirect_output_append(t_redirect *redir)
// {
    
//     if (!redir || !redir->file)
//         return (1);
  
//     int fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
//     // printf("open fd return %d\n", fd);
//     if (fd == -1)
//     {
//         ft_printf_fd(2, "minishell: failed to open '%s': %s\n",
//                     redir->file, strerror(errno));
//         return (1);
//     }
//     // printf("redirect append work 1\n");

//     // if (fcntl(STDOUT_FILENO, F_GETFD) == -1)
//     // {
//     //     printf("STDOUT is closed or invalid\n");
//     //     close(fd);
//     //     return (1);
//     // }
//     // printf("check before checking fd open");

//     // if (fcntl(fd, F_GETFD) == -1) {
//     //     ft_printf_fd(2, "minishell: file descriptor %d is invalid: %s\n", fd, strerror(errno));
//     //     close(fd);
//     //     return (1);
//     // }
//     // printf("check before dup2 not failed");

//     if (dup2(fd, STDOUT_FILENO) == -1)
//     {
//         printf("dup2 failed\n");
//         close(fd);
//         // ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
//         return (1);
//     }
    
//     printf("dup2 not failed");

//     close(fd);
//     printf("redirect append work 3\n");
//     return (0);
// }

// original function

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
    {
        return display_error_errno(mshell, "heredoc: no temporary file", 0);
        //return (mshell->exit_code == 130) ? 1 : display_error_errno(mshell, "heredoc: no temporary file", 0);
    }
    fd = open(redir->tmp_file, O_RDONLY);
    if (fd == -1)
        return display_error_errno(mshell, redir->tmp_file, 1);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return display_error_errno(mshell, "dup2", 1);
    }
    close(fd);
    // unlink(redir->tmp_file); --> free in main
    // free(redir->tmp_file);
    // redir->tmp_file = NULL;
    return 0;
}

int exe_redirection(t_redirect *redir, t_shell *mshell)
{
    t_redirect *current = redir;

    if (!mshell)
    {
        ft_printf_fd(2, "minishell: internal error: null shell pointer\n");
        return (1);
    }

    //printf("exe_redirection worked\n");

    while (current)
    {
        if (is_ambiguous_redirect(mshell, current) != 0)
        {
            // printf("ambiguous redirect");
            mshell->exit_code = 1;
            return (mshell->exit_code);
        }
        if (current->type == REDIR_IN)
            mshell->exit_code = redirect_input(current);
        else if (current->type == REDIR_OUT)
            mshell->exit_code = redirect_output(current);
        else if (current->type == REDIR_APPEND)
        {
            // printf("APPEND redirect\n");
            mshell->exit_code = redirect_output_append(current);
        }
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

