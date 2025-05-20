#include "../../includes/shell.h"

// int builtin_unset(t_shell *mshell, char **token)
// {
//     if (!mshell || !token || !token[0])
//     {
//         ft_printf_fd(2, "minishell: pwd: internal error\n");
//         mshell->exit_code = 1;
//         return (0);
//     }
//     if (token[1] != NULL && token[1][0] == '-' )
//     {
//         ft_printf_fd(2, "minishell: pwd: options aren't supported\n");
//         ft_printf_fd(2, "pwd: usage: pwd\n");
//         mshell->exit_code = 1;
//         return (0);
//     }
//     return (1);   
// }

// ///new code
// #include "../../includes/shell.h"

static int is_valid_var_name(const char *name)
{
    size_t i = 0;
    if (!name || !name[0] || (!ft_isalpha(name[0]) && name[0] != '_'))
        return (0);
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int builtin_unset(t_shell *mshell, char **token)
{
    size_t i = 1;

    if (!mshell || !token || !token[0])
    {
        ft_printf_fd(2, "minishell: unset: internal error\n");
        mshell->exit_code = 1;
        return (1);
    }
    if (!token[1])
    {
        mshell->exit_code = 0;
        return (0);
    }
    while (token[i])
    {
        if (token[i][0] == '-')
        {
            ft_printf_fd(2, "minishell: unset: %s: invalid option\n", token[i]);
            mshell->exit_code = 2;
            return (2);
        }
        if (!is_valid_var_name(token[i]))
        {
            ft_printf_fd(2, "minishell: unset: `%s': not a valid identifier\n", token[i]);
            mshell->exit_code = 1;
        }
        else
            env_remove(mshell, token[i]);
        i++;
    }
    return (mshell->exit_code);
}
