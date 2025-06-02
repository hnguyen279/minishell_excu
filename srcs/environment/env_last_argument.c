#include "../../includes/shell.h"

int env_set_last_argument(t_shell *mshell, char **cmd)
{
    if (!mshell || !cmd || !cmd[0])
        return 1;

    size_t i = 0;
    while (cmd[i])
        i++;

    if (i == 0)
        return 0;

    char *last_arg = cmd[i - 1];
     if (!last_arg[0] ||
        strcmp(last_arg, "<") == 0 ||
        strcmp(last_arg, ">") == 0 ||
        strcmp(last_arg, "<<") == 0 ||
        strcmp(last_arg, ">>") == 0)
    {
        return 0;
    }
    return (env_add(mshell, "_", last_arg));
}
