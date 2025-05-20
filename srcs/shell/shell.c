#include "../../includes/shell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>



// init t_shell

void shell_cleanup(t_shell *mshell)
{
    if (!mshell)
        return;
    if (mshell->env_last_cmd)
    {
        free(mshell->env_last_cmd);
        mshell->env_last_cmd = NULL;
    }
    if (mshell->envp)
        env_free(mshell);
    // add more
}

//////
int init_shell(t_shell *mshell, char **envp)
{
    if (!mshell)
        return (1);
    mshell->exit_code = 0;
    mshell->heredoc_index = 0;
    mshell->has_pipe = 0;
    mshell->env_last_cmd = NULL;

    // If no envp passed, add defaults FIRST
    if (!envp || !envp[0])
    {
        if (env_add(mshell, "PATH", "/usr/local/bin:/usr/bin:/bin"))
            return (1);
        if (env_add(mshell, "HOME", "/home/user"))
            return (1);
    }

    if (init_env(mshell, envp)) // safe now
        return (1);

    // Now SHLVL logic
    char *shlvl_val = env_find_value(mshell, "SHLVL");
    int shlvl = 1;
    if (shlvl_val && shlvl_val[0] != '\0')
    {
        int i = 0;
        while (shlvl_val[i] && ft_isdigit(shlvl_val[i]))
            i++;
        if (shlvl_val[i] == '\0')
            shlvl = atoi(shlvl_val) + 1;
    }

    if (shlvl < 0 || shlvl > 999)
        shlvl = 1;

    char *shlvl_str = ft_itoa(shlvl);
    if (!shlvl_str)
        return (1);
    if (env_add(mshell, "SHLVL", shlvl_str))
    {
        free(shlvl_str);
        env_free(mshell);
        return (1);
    }
    free(shlvl_str);

    return (0);
}

