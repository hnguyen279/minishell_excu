#include "../../includes/shell.h"

int builtin_unset(t_shell *mshell, char **token)
{
    if (!mshell || !token || !token[0])
    {
        //ft_printf_fd(2, "minishell: pwd: internal error\n");
        mshell->exit_code = 1;
        return (0);
    }
    if (token[1] != NULL && token[1][0] == '-' )
    {
        //ft_printf_fd(2, "minishell: pwd: options aren't supported\n");
        //ft_printf_fd(2, "pwd: usage: pwd\n");
        mshell->exit_code = 1;
        return (0);
    }
    return (1);   
}