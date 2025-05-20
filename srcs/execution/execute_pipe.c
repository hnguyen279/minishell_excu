#include "../../includes/shell.h"

static void handle_dup2_error(int fd_to_close)
{
    ft_printf_fd(2, "minishell: dup2: %s\n", strerror(errno));
    close(fd_to_close);
    exit(1);
}

static void execute_child(t_shell *shell, t_ast *ast, int *pipe_fd, int left)
{
    setup_signals(shell, MODE_CHILD);
    if (!ast)
        exit(1);
    if (ast->redirects && exe_redirection(ast->redirects, shell) != 0)
        exit(shell->exit_code);
    if (left)
    {
        close(pipe_fd[FD_READ]);
        if (dup2(pipe_fd[FD_WRITE], STDOUT_FILENO) == -1)
            handle_dup2_error(pipe_fd[FD_WRITE]);
        close(pipe_fd[FD_WRITE]);
        execute_ast(ast->left, shell);
    }
    else
    {
        close(pipe_fd[FD_WRITE]);
        if (dup2(pipe_fd[FD_READ], STDIN_FILENO) == -1)
            handle_dup2_error(pipe_fd[FD_READ]);
        close(pipe_fd[FD_READ]);
        execute_ast(ast->right, shell);
    }
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

    if (!ast->left || !ast->right)
        return (shell->exit_code = 1);
    if (pipe(pipe_fd) == -1)
        return (perror("minishell: pipe"), shell->exit_code = 1);
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
    return shell->exit_code;
}



