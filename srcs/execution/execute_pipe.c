#include "../../includes/shell.h"

static void setup_pipe_redirection(int pipe_fd_src, int std_fd_dst)
{
    if (dup2(pipe_fd_src, std_fd_dst) == -1)
    {
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        close(pipe_fd_src);
        exit(1);
    }
    close(pipe_fd_src);
}

static void execute_child(t_shell *mshell, t_ast *ast, int *pipe_fd, int left)
{
    t_ast *child;

    setup_signals(mshell, MODE_CHILD);
    if (!ast)
        exit(1);
    if (left)
        child = ast->left;
    else
        child = ast->right;  
    if (!child)
        exit(1);
    if (child->redirects && exe_redirection(child->redirects, mshell) != 0)
        exit(mshell->exit_code);
    if (left)
    {
        close(pipe_fd[FD_READ]);
        setup_pipe_redirection(pipe_fd[FD_WRITE], STDOUT_FILENO); //1
        //setup_pipe_redirection(pipe_fd, FD_WRITE, pipe_fd[FD_WRITE]);
    }
    else
    {
        close(pipe_fd[FD_WRITE]);
        setup_pipe_redirection(pipe_fd[FD_READ], STDIN_FILENO); //0
        //setup_pipe_redirection(pipe_fd, FD_READ, pipe_fd[FD_READ]);
    }
    execute_ast(child, mshell);
    exit(mshell->exit_code);
}

static int init_child(int *pipe_fd, pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        perror("minishell: fork");
        close(pipe_fd[FD_READ]);
        close(pipe_fd[FD_WRITE]);
        return -1;
    }
    return 0;
}

int wait_command(t_shell *mshell, pid_t pid, int *status, int update_exit_code)
{
    int sig;

    if (waitpid(pid, status, 0) == -1)
        return display_error_errno(mshell, "waitpid", 1);
    if (!update_exit_code)
        return (0);
    if (WIFEXITED(*status))
        mshell->exit_code = WEXITSTATUS(*status);
    else if (WIFSIGNALED(*status))
    {
        sig = WTERMSIG(*status);
        mshell->exit_code = 128 + sig;
        if (sig == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 20);
        else if (sig == SIGINT)
            write(STDERR_FILENO, "\n", 1);
    }
    else
        mshell->exit_code = 1;
    return (mshell->exit_code);
}

int execute_pipe(t_ast *ast, t_shell *mshell)
{
    int pipe_fd[2];
    pid_t pid[2];
    int status[2];

    if (!ast || !ast->left || !ast->right)
        return display_error_errno(mshell, "syntax error near unexpected token `|'", 0);
    if (pipe(pipe_fd) == -1)
        return display_error_errno(mshell, "pipe", 1);
    if (init_child(pipe_fd, &pid[0]) == -1)
        return (mshell->exit_code);
    if (pid[0] == 0)
        execute_child(mshell, ast, pipe_fd, 1);
    if (init_child(pipe_fd, &pid[1]) == -1)
        return (mshell->exit_code);
    if (pid[1] == 0)
        execute_child(mshell, ast, pipe_fd, 0);
    close(pipe_fd[FD_READ]);
    close(pipe_fd[FD_WRITE]);
    wait_command(mshell, pid[0], &status[0], 0);     // only wait, not update exit_code
    wait_command(mshell, pid[1], &status[1], 1);    // wait and update exit_code
    return mshell->exit_code;
}
