#include "minishell.h"

int builtin_pwd(t_shell *mshell, char **token)
{
    const char *pwd_value;
    char *current_pwd;

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

