#include "../../includes/shell.h"

static void setup_pipe_redirection(int *pipe_fd, int std_fd, int fd_to_close)
{
    if (dup2(pipe_fd[std_fd], std_fd) == -1)
    {
        ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
        close(fd_to_close);
        exit(1);
    }
    close(pipe_fd[std_fd]);
}

static void execute_child(t_shell *shell, t_ast *ast, int *pipe_fd, int left)
{
    t_ast *child;

    setup_signals(shell, MODE_CHILD);
    if (!ast)
        exit(1);
    if (left)
        child = ast->left;
    else
        child = ast->right;  
    if (!child)
        exit(1);
    if (child->redirects && exe_redirection(child->redirects, shell) != 0)
        exit(shell->exit_code);
    if (left)
    {
        close(pipe_fd[FD_READ]);
        setup_pipe_redirection(pipe_fd, FD_WRITE, pipe_fd[FD_WRITE]);
    }
    else
    {
        close(pipe_fd[FD_WRITE]);
        setup_pipe_redirection(pipe_fd, FD_READ, pipe_fd[FD_READ]);
    }
    execute_ast(child, shell);
    exit(shell->exit_code);
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

int execute_pipe(t_ast *ast, t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid[2];
    int status[2];

    if (!ast || !ast->left || !ast->right)
    {
        ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n");
        return (shell->exit_code = 1);
    }
    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe");
        return (shell->exit_code = 1);
    }
    if (init_child(pipe_fd, &pid[0]) == -1)
        return shell->exit_code;
    if (pid[0] == 0)
        execute_child(shell, ast, pipe_fd, 1);
    if (init_child(pipe_fd, &pid[1]) == -1)
        return shell->exit_code;
    if (pid[1] == 0)
        execute_child(shell, ast, pipe_fd, 0);
    close(pipe_fd[FD_READ]);
    close(pipe_fd[FD_WRITE]);
    waitpid(pid[0], &status[0], 0);
    waitpid(pid[1], &status[1], 0);
    if (WIFEXITED(status[1]))
        shell->exit_code = WEXITSTATUS(status[1]);
    else if (WIFSIGNALED(status[1]))
        shell->exit_code = 128 + WTERMSIG(status[1]);
    else
        shell->exit_code = 1;
    if (ast->right && ast->right->cmd)
        env_set_last_argument(shell, ast->right->cmd);
    else if (ast->left && ast->left->cmd)
        env_set_last_argument(shell, ast->left->cmd);
    return shell->exit_code;
}
