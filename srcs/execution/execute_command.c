
#include "../../includes/shell.h"

static int is_builtin(char *cmd)
{
	if (!cmd)
		return 0;
	if (
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "env") == 0 || 
        ft_strcmp(cmd, "exit") == 0 || 
        ft_strcmp(cmd, "export") == 0

	)
		return 1;
	return 0;
}

// static int execute_builtin(t_shell *mshell, char **token)
// {
// 	if (ft_strcmp(token[0], "cd") == 0)
// 		return builtin_cd(mshell, token);
// 	else if (ft_strcmp(token[0], "echo") == 0)
// 		return builtin_echo(mshell, token);
// 	else if (ft_strcmp(token[0], "pwd") == 0)
// 		return builtin_pwd(mshell, token);
// 	else if (ft_strcmp(token[0], "unset") == 0)
// 		return builtin_unset(mshell, token);
// 	else if (ft_strcmp(token[0], "env") == 0)
// 		return builtin_env(mshell, token);
//     else if (ft_strcmp(token[0], "exit") == 0)
//         return builtin_exit(mshell, token);
// 	else
// 		return -1; 
// }

static int execute_builtin(t_shell *mshell, char **token)
{
    //debug
    // printf("execute_builtin\n");
    // for (int i = 0; token[i]; i++)
    // {
    //     printf("token[%d] = [%s]\n", i, token[i]);
    // }
	if (ft_strcmp(token[0], "cd") == 0)
		mshell->exit_code = builtin_cd(mshell, token);
	else if (ft_strcmp(token[0], "echo") == 0)
		mshell->exit_code = builtin_echo(mshell, token);
	else if (ft_strcmp(token[0], "pwd") == 0)
		mshell->exit_code = builtin_pwd(mshell, token);
	else if (ft_strcmp(token[0], "unset") == 0)
		mshell->exit_code = builtin_unset(mshell, token);
	else if (ft_strcmp(token[0], "env") == 0)
		mshell->exit_code = builtin_env(mshell, token);
    else if (ft_strcmp(token[0], "exit") == 0)
        builtin_exit(mshell, token);
    else if (ft_strcmp(token[0], "export") == 0)
	    builtin_export(mshell, token);
	else
		return (1); 
	return mshell->exit_code;
}

static int wait_command(t_shell *mshell, pid_t pid, int *status)
{
    if (waitpid(pid, status, 0) == -1)
        return display_error_errno(mshell, "waitpid", 1);

    if (WIFEXITED(*status))
        mshell->exit_code = WEXITSTATUS(*status);
    // else if (WIFSIGNALED(*status))
    //     mshell->exit_code = 128 + WTERMSIG(*status);
    if (WIFSIGNALED(*status))
    {
        int sig = WTERMSIG(*status);
        mshell->exit_code = 128 + sig;
        if (sig == SIGQUIT)
            ft_printf_fd(2, "Quit (core dumped)\n");
        else if (sig == SIGINT)
            ft_printf_fd(2, "\n");
    }
    return mshell->exit_code;
}

static void run_command_child(t_ast *node, t_shell *mshell, char *cmd_path)
{
    setup_signals(mshell, MODE_CHILD);

    if (node->redirects && exe_redirection(node->redirects, mshell) != 0)
        exit(mshell->exit_code);
    // for (int i = 0; node->cmd[i]; i++)
    //     printf("cmd[%d] = %s\n", i, node->cmd[i]);
    if (execve(cmd_path, node->cmd, mshell->envp) == -1)
    {
        ft_printf_fd(2, "minishell: %s: %s\n", node->cmd[0], strerror(errno));
        free(cmd_path);
        if (errno == ENOENT)
            exit(127);
        if (errno == EACCES)
            exit(126);
        exit(1);
    }
}

static int fork_and_exec(t_ast *node, t_shell *mshell, char *cmd_path)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        free(cmd_path);
		ft_printf_fd(2, "minishell: fork: %s\n", strerror(errno));
        mshell->exit_code = 1;
        return (mshell->exit_code);
    }
    if (pid == 0)
        run_command_child(node, mshell, cmd_path);


    free(cmd_path);
    return (wait_command(mshell, pid, &status));
}


static int execute_with_redirect(t_ast *node, t_shell *mshell, int is_builtin)
{
    int in_fd = dup(STDIN_FILENO);
    int out_fd = dup(STDOUT_FILENO);

    if (in_fd == -1 || out_fd == -1)
    {
        perror("minishell: dup failed");
        if (in_fd != -1)
            close(in_fd);
        if (out_fd != -1)
            close(out_fd);
        mshell->exit_code = 1;
        return (mshell->exit_code);
    }
    if (node->redirects && exe_redirection(node->redirects, mshell) != 0)
    {
        if (dup2(in_fd, STDIN_FILENO) == -1 || dup2(out_fd, STDOUT_FILENO) == -1)
            perror("minishell: dup2 restore failed");
        close(in_fd);
        close(out_fd);
        return (mshell->exit_code);
    }
    if (is_builtin)
        mshell->exit_code = execute_builtin(mshell, node->cmd);
    if (!mshell->has_pipe && node->cmd && node->cmd[0])
        env_set_last_argument(mshell, node->cmd);
    if (dup2(in_fd, STDIN_FILENO) == -1 || dup2(out_fd, STDOUT_FILENO) == -1)
        perror("minishell: dup2 restore failed");
    close(in_fd);
    close(out_fd);
    return (mshell->exit_code);
}

// static int execute_with_redirect(t_ast *node, t_shell *mshell, int is_builtin)
// {
//     int in_fd = dup(STDIN_FILENO);
//     int out_fd = dup(STDOUT_FILENO);
//     int redirect_ok = 1;
//     int builtin_ok = 0;

//     if (in_fd == -1 || out_fd == -1)
//     {
//         perror("minishell: dup failed");
//         if (in_fd != -1)
//             close(in_fd);
//         if (out_fd != -1)
//             close(out_fd);
//         mshell->exit_code = 1;
//         return mshell->exit_code;
//     }
//     if (node->redirects && exe_redirection(node->redirects, mshell) != 0)
//         redirect_ok = 0;
//     if (is_builtin)
//     {
//         if (redirect_ok)
//         {
//             mshell->exit_code = execute_builtin(mshell, node->cmd);
//             builtin_ok = 1;
//         }
//         else
//             builtin_ok = 0;
//     }
//     if (redirect_ok && builtin_ok && !mshell->has_pipe && node->cmd && node->cmd[0])
//         env_set_last_argument(mshell, node->cmd);
//     if (dup2(in_fd, STDIN_FILENO) == -1 || dup2(out_fd, STDOUT_FILENO) == -1)
//         perror("minishell: dup2 restore failed");
//     close(in_fd);
//     close(out_fd);
//     return mshell->exit_code;
// }


int execute_command(t_ast *node, t_shell *mshell)
{
    char *cmd_path;

    if (!node->cmd || !node->cmd[0])
    {
        if (node->redirects)
        {
            //printf("[DEBUG] Redirect-only command\n");
            return (execute_with_redirect(node, mshell, 0)); //fix because > file not work
        }
        mshell->exit_code = display_error_cmd(NULL);
        return (mshell->exit_code);
    }
    if (is_builtin(node->cmd[0]))
        return (execute_with_redirect(node, mshell, 1));
    cmd_path = find_cmd_path(mshell, node->cmd[0]);
    if (!cmd_path)
    {
        return mshell->exit_code;

    }
    mshell->exit_code = fork_and_exec(node, mshell, cmd_path);
    if (!mshell->has_pipe && node->cmd && node->cmd[0])
        env_set_last_argument(mshell, node->cmd);
    return (mshell->exit_code);
}

