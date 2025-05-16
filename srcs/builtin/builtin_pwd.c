#include "../../includes/shell.h"

//note check cd --help or pwd --help or cd -- / pwd --help /pwd --/ pwd -

static int pwd_check_args(t_shell *mshell, char **token)
{
    if (!mshell || !token || !token[0])
    {
        //ft_printf_fd(2, "minishell: pwd: internal error\n");
        mshell->exit_code = 1;
        return (0);
    }
    if (token[1] != NULL)
    {
        if (token[1][0] == '-' && token[1][1] != '\0')
        {
            //ft_printf_fd(2, "minishell: pwd: options aren't supported\n");
            //ft_printf_fd(2, "pwd: usage: pwd\n");
            mshell->exit_code = 2;
            return (0);
        }
        else if (token[2] != NULL)
        {
            //ft_printf_fd(2, "minishell: pwd: too many arguments\n");
            mshell->exit_code = 1;
            return (0);
        }
    }
    return (1);
}

int builtin_pwd(t_shell *mshell, char **token)
{
    const char *pwd_value;
    char *current_pwd;

    if (!pwd_check_args(mshell, token))
        return mshell->exit_code;
    pwd_value = env_find_value(mshell, "PWD");
    if (pwd_value)
    {
        printf("%s\n", pwd_value);
        mshell->exit_code = 0;
        return mshell->exit_code;
    }
    current_pwd = getcwd(NULL, 0);
    if (!current_pwd)
    {
        perror("minishell: getcwd");
        mshell->exit_code = 1;
        return mshell->exit_code;
    }
    printf("%s\n", current_pwd);
    free(current_pwd);
    mshell->exit_code = 0;
    return mshell->exit_code;
}
