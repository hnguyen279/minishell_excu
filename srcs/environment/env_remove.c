#include "../../includes/shell.h"

int env_remove(t_shell *mshell, const char *key)
{
    size_t i;
    size_t j;

    i = 0;
    if (!mshell || !env_find_variable(mshell, key, &i))
        return (0);
    free(mshell->envp[i]);
    j = i + 1;
    while (mshell->envp[j])
    {
        mshell->envp[i] = mshell->envp[j];
        i++;
        j++;
    }
    mshell->envp[i] = NULL;
    return (1);
}
