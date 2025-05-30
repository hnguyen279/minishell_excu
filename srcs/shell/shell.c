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

int init_shell(t_shell *mshell, char **envp)
{
    ft_memset(mshell, 0, sizeof(t_shell));
    mshell->envp = env_dup(envp);
    if (!mshell->envp)
    {
        ft_printf_fd(2, "init_shell: failed to duplicate env\n");
        return (1);
    }
    if (!env_find_value(mshell, "OLDPWD"))
		env_add(mshell, "OLDPWD", NULL);
    char *shlvl_val = env_find_value(mshell, "SHLVL");
    int shlvl = 1;
    if (shlvl_val && shlvl_val[0] != '\0')
    {
        int i = 0;
        while (shlvl_val[i] && ft_isdigit(shlvl_val[i]))
            i++;
        if (shlvl_val[i] == '\0')
            shlvl = ft_atoi(shlvl_val) + 1;
    }
    if (shlvl < 0 || shlvl > 999)
        shlvl = 1;
    char *shlvl_str = ft_itoa(shlvl);
    if (!shlvl_str)
    {
        ft_printf_fd(2, "init_shell: malloc failed (SHLVL)\n");
        return (1);
    }
    if (env_add(mshell, "SHLVL", shlvl_str) != 0)
    {
        free(shlvl_str);
        ft_printf_fd(2, "init_shell: failed to set SHLVL\n");
        return (1);
    }
    free(shlvl_str);
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("minishell: getcwd");
        return (1);
    }
    if (env_add(mshell, "PWD", cwd) != 0)
    {
        free(cwd);
        ft_printf_fd(2, "init_shell: failed to set PWD\n");
        return (1);
    }
    free(cwd);
    //env_remove(mshell, "OLDPWD");
    return (0);
}



// int init_shell(t_shell *mshell, char **envp)
// {
//     ft_memset(mshell, 0, sizeof(t_shell));
//     mshell->envp = env_dup(envp);
//     if (!mshell->envp)
//     {
//         ft_printf_fd(2, "init_shell: failed to duplicate env\n");
//         return (1);
//     }
//     // SHLVL
//     char *shlvl_val = env_get(mshell->envp, "SHLVL", 0);
//     int shlvl = ft_atoi(shlvl_val);
//     shlvl++; 
//     char *shlvl_str = ft_itoa(shlvl);
//     if (!shlvl_str)
//     {
//         ft_printf_fd(2, "init_shell: malloc failed\n");
//         return (1);
//     }
//     if (env_set(mshell, "SHLVL", shlvl_str) != 0)
//     {
//         free(shlvl_str);
//         ft_printf_fd(2, "init_shell: failed to set SHLVL\n");
//         return (1);
//     }
//     free(shlvl_str);
//     char *cwd = getcwd(NULL, 0);
//     if (!cwd)
//     {
//         perror("minishell: getcwd");
//         return (1);
//     }
//     if (env_add(mshell, "PWD", cwd) != 0)
//     {
//         free(cwd);
//         ft_printf_fd(2, "init_shell: failed to set PWD\n");
//         return (1);
//     }
//     free(cwd);
//     env_remove(mshell, "OLDPWD");
//     return (0);
// }

