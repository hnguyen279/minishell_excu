#include "../../includes/shell.h"

int builtin_env(t_shell *mshell, char **token)
{
    if (!mshell || !token || !token[0])
    {
        ft_printf_fd(2, "minishell: builtin_env: Invalid parameter(s)\n");
        mshell->exit_code = 1;
        return mshell->exit_code;
    }
    if (token[1] != NULL)
    {
        if (token[1][0] == '-')
        {
            ft_printf_fd(2, "env: unrecognized option '%s'\n", token[1]);
            mshell->exit_code = 125;
            return mshell->exit_code;
        }
        else
        {
            ft_printf_fd(2, "env: '%s': No such file or directory\n", token[1]);
            mshell->exit_code = 127;
            return mshell->exit_code;
        }
    }
    env_print(mshell->envp);
    mshell->exit_code = 0;
    return mshell->exit_code;
}
