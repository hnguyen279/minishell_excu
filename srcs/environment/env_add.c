#include "../../includes/shell.h"

static char *make_env_entry(const char *key, const char *value)
{
    char *entry;
    size_t len;

    len = ft_strlen(key) + 1; // for '=' or '\0'
    if (value)
        len += ft_strlen(value);
    entry = ft_calloc(1, len);
    if (!entry)
        return (NULL);
    ft_strlcat(entry, key, len);
    if (value)
    {
        ft_strlcat(entry, "=", len);
        ft_strlcat(entry, value, len);
    }
    return (entry);
}

static int append_env_variable(t_shell *mshell, char *entry)
{
    size_t env_len;

    env_len = 0;
    while (mshell->envp[env_len])
        env_len++;
    mshell->envp = realloc_env(mshell->envp, env_len);
    if (!mshell->envp)
    {
        free(entry);
        return display_error_errno(mshell, "env_add: realloc failed", 0);
    }
    mshell->envp[env_len] = entry;
    mshell->envp[env_len + 1] = NULL;
    if (ft_strncmp(entry, "_=", 2) == 0)
        env_swap_last(mshell->envp);
    return (0);
}

int env_add(t_shell *mshell, const char *key, const char *value)
{
    char *entry;
    size_t i;

    i = 0;
    if (!mshell || !key)
        return 1;
    entry = make_env_entry(key, value);
    if (!entry)
        return display_error_errno(mshell, "env_add: malloc failed", 0);
    if (env_find_variable(mshell, key, &i))
    {
        free(mshell->envp[i]);
        mshell->envp[i] = entry;
        if (ft_strncmp(entry, "_=", 2) == 0)
            env_swap_last(mshell->envp);
        return (0);
    }
    return append_env_variable(mshell, entry);
}
