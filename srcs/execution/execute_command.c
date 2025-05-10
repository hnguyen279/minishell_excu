
#include "minishell.h"

static int is_builtin(char *cmd)
{
	if (!cmd)
		return 0;
	return (
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0
	);
}

static int execute_builtin(t_shell *mshell, char **token)
{

	if (ft_strcmp(token[0], "cd") == 0)
		mshell->exit_code = builtin_cd(shell, token);
	else if (ft_strcmp(token[0], "echo") == 0)
		mshell->exit_code = builtin_echo(shell, token);
	else if (ft_strcmp(token[0], "exit") == 0)
		mshell->exit_code = builtin_exit(shell, token);
	else if (ft_strcmp(token[0], "pwd") == 0)
		mshell->exit_code = builtin_pwd(shell, token);
	else if (ft_strcmp(token[0], "export") == 0)
		mshell->exit_code = builtin_export(shell, token);
	else if (ft_strcmp(token[0], "unset") == 0)
		mshell->exit_code = builtin_unset(shell, token);
	else if (ft_strcmp(token[0], "env") == 0)
		mshell->exit_code = builtin_env(shell, token);
	else
		return (1); 

	return mshell->exit_code;
}

static int wait_command(t_shell *mshell, pid_t pid, int status)
{
    if (waitpid(pid, &status, 0) == -1)
    {
        ft_printf_fd(2, "minishell: waitpid: %s\n", strerror(errno));
        mshell->exit_code = 1;
        return (1);
    }
    if (WIFEXITED(status))
        mshell->exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        mshell->exit_code = 128 + WTERMSIG(status);
    return (mshell->exit_code);
}

static void run_command_child(t_ast *node, t_shell *mshell, char *cmd_path)
{
    setup_signals(CHILD_STATE);
    if (execve(cmd_path, node->cmd, mshell->env) == -1)
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
		return (ft_printf_fd(2, "minishell: fork: %s\n", strerror(errno)), mshell->exit_code = 1);
    }
    if (pid == 0)
        run_command_child(node, mshell, cmd_path);
    free(cmd_path);
    return (wait_command(mshell, pid, status));
}


int execute_command(t_ast *node, t_shell *mshell)
{
    char *cmd_path;

    if (!node->cmd || !node->cmd[0])
    {
        mshell->exitcode = display_error(NULL);
        return (mshell->exitcode);
    }
    if (node->redirects && exe_redirection(node->redirects, mshell) != 0)
        return (mshell->exit_code);
    cmd_path = find_cmd_path(mshell, node->cmd[0]);
    if (!cmd_path)
        return (mshell->exitcode);
    if (is_builtin(node->cmd[0]))
    {
        mshell->exit_code = execute_builtin(mshell, node->cmd);
        free(cmd_path);
        return (mshell->exit_code);
    }
    return (fork_and_exec(node, mshell, cmd_path));
}

