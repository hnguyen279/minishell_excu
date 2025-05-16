#include "../../includes/shell.h"

int execute_ast(t_ast *node, t_shell *mshell)
{
    if (!node)
        return 0;
    if (process_heredocs(mshell, node) != 0)
        return mshell->exit_code;
    if (node->type == NODE_PIPE)
        return execute_pipe(node, mshell);
    else if (node->type == NODE_CMD)
        return execute_command(node, mshell);
    ft_printf_fd(2, "minishell: unsupported node type\n");
    return (mshell->exit_code = 1);
}