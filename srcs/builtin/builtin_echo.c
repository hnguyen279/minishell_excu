#include "../../includes/shell.h"
//NOT echo $? or $HOME or $USER

int builtin_echo(t_shell *mshell, char **token)
{
    int i;
    int newline;
    int j;

    // Debug builtin
    printf("builtin_echo\n");
    for (int i = 0; token[i]; i++)
        printf("token[%d] = [%s]\n", i, token[i]);
    newline = 1;
    i = 1;
    if (token[i] && ft_strncmp(token[i], "-n", 2) == 0)
    {
        j = 2;
        while (token[i][j] != '\0' && token[i][j] == 'n')
            j++;
        if (token[i][j] == '\0')  // -n, -nnn, ...
        {
            newline = 0;
            i++;
        }
    }
    while (token[i])
    {
        printf("%s", token[i]);
        if (token[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    mshell->exit_code = 0;
    return mshell->exit_code;
}
