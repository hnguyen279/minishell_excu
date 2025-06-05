#include "../../includes/shell.h"
//NOT echo $? or $HOME or $USER

int builtin_echo(t_shell *mshell, char **token)
{
    int i;
    int newline;
    int j;

    // Debug builtin
    // printf("builtin_echo\n");
    // for (int i = 0; token[i]; i+'+)
    //     printf("token[%d] = [%s]'\n", i, token[i]);
    newline = 1;
    i = 1;
    while (token[i])
    {
        if (token[i][0] != '-' || token[i][1] != 'n')
            break;
        j = 2;
        while (token[i][j] == 'n')
            j++;
        if (token[i][j] != '\0')
            break;
        newline = 0;
        i++;
    }
    while (token[i])
    {
        write(STDOUT_FILENO, token[i], ft_strlen(token[i]));
        if (token[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (newline)
        write(STDOUT_FILENO, "\n", 1);
    mshell->exit_code = 0;
    return (mshell->exit_code);
}
