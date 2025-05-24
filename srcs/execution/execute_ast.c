#include "../../includes/shell.h"


int execute_ast(t_ast *node, t_shell *mshell)
{
    if (!node)
        return 0;
    if (node->type == NODE_PIPE)
    {
        //print_error(" execute ast 2\n");
        mshell->has_pipe = 1;
        return execute_pipe(node, mshell);
    }
    else if (node->type == NODE_CMD)
    {
        //print_error("execute ast 3\n");
        mshell->has_pipe = 0;
        return execute_command(node, mshell);
    }
    ft_printf_fd(2, "minishell: unsupported node type\n");
    return (mshell->exit_code = 1);
}
