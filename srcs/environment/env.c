
#include "../../includes/shell.h"

void ft_free_null(char ***array)
{
	size_t i = 0;

	if (!array || !*array)
		return;

	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}


void env_free(t_shell *mshell)
{
    size_t i;

    if (!mshell || !mshell->envp)
        return;
    i = 0;
    while (mshell->envp[i])
    {
        free(mshell->envp[i]);
        mshell->envp[i] = NULL;
        i++;
    }
    free(mshell->envp);
    mshell->envp = NULL;
}


char **env_dup(char **envp)
{
    char **res;
    size_t i;
    size_t len;

    if (!envp)
        return (NULL);
    len = 0;
    while (envp[len])
        len++;
    res = ft_calloc(len + 1, sizeof(char *));
    if (!res)
        return (NULL);
    i = 0;
    while (i < len)
    {
        res[i] = ft_strdup(envp[i]);
        if (!res[i])
            return (ft_free_null(&res), NULL);
        i++;
    }
    return (res);
}

static size_t strlen_until_char(const char *s, const char ch)
{
    size_t len;

    len = 0;
    while (*s && *s != ch)
    {
        len++;
        s++;
    }
    return (len);
}

char *env_find_variable(t_shell *mshell, const char *key, size_t *i) //found key and value
{
    size_t key_len;
    size_t env_key_len;

    if (!mshell || !mshell->envp || !key || !i)
        return (NULL);
    key_len = ft_strlen(key);
    *i = 0;
    while (mshell->envp[*i])
    {
        env_key_len = strlen_until_char(mshell->envp[*i], '=');
        if (key_len == env_key_len && ft_strncmp(mshell->envp[*i], key, key_len) == 0)
            return (mshell->envp[*i]);
        (*i)++;
    }
    return (NULL);
}

char *env_find_value(t_shell *mshell, const char *key) // found value
{
    size_t i;
    size_t key_len;
    size_t env_key_len;

    if (!mshell || !mshell->envp || !key)
        return (NULL);
    if (*key == '$')
        key++;
    key_len = ft_strlen(key);
    i = 0;
    while (mshell->envp[i])
    {
        env_key_len = strlen_until_char(mshell->envp[i], '=');
        if (key_len == env_key_len && ft_strncmp(mshell->envp[i], key, key_len) == 0)
            return (mshell->envp[i] + key_len + 1);
        i++;
    }
    return (NULL);
}

static char **realloc_env(char **envp, size_t len)
{
    char **res;
    size_t i;

    res = ft_calloc(len + 2, sizeof(char *));
    if (!res)
        return (NULL);
    i = 0;
    while (i < len)
    {
        res[i] = ft_strdup(envp[i]);
        if (!res[i])
        {
            ft_free_null(&res);
            return (NULL);
        }
        i++;
    }
    ft_free_null(&envp);
    return (res);
}



int env_remove(t_shell *mshell, const char *key) //unset
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

void env_sort(char **envp, size_t len)
{
    size_t i;
    size_t j;
    char *temp;

    i = 0;
    while (i < len)
    {
        j = i + 1;
        while (j < len)
        {
            if (ft_strcmp(envp[i], envp[j]) > 0)
            {
                temp = envp[i];
                envp[i] = envp[j];
                envp[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void env_swap_last(char **envp)
{
    char *tmp;
    size_t len;

    if (!envp)
        return;
    len = 0;
    while (envp[len])
        len++;
    if (len > 1 && ft_strncmp(envp[len - 2], "_=", 2) == 0)
    {
        tmp = envp[len - 2];
        envp[len - 2] = envp[len - 1];
        envp[len - 1] = tmp;
    }
}

void	env_print(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			printf("%s\n", envp[i]);
		}
		i++;
	}
}

// int env_set_last_argument(t_shell *mshell, char **cmd)
// {
//     char *last_cmd;
//     size_t i;

//     if (!mshell || !cmd)
//         return (1);
//     i = 0;
//     while (cmd[i])
//         i++;
//     if (i > 0 && !mshell->has_pipe)
//     {
//         last_cmd = cmd[i - 1];
//         return (env_add(mshell, "_", last_cmd));
//     }
//     return (1);
// }

int env_set_last_argument(t_shell *mshell, char **cmd)
{
    char *last_cmd = NULL;
    size_t i = 0;

    if (!mshell)
        return 1;
    if (cmd && cmd[0])
    {
        while (cmd[i])
            i++;
        if (i > 0)
            last_cmd = cmd[i - 1];
    }
    else if (mshell->env_last_cmd)
    {
        last_cmd = mshell->env_last_cmd;
    }

    if (last_cmd)
        return env_add(mshell, "_", last_cmd);

    return 0;
}

int env_backup_last_argument(t_shell *mshell, char **cmd)
{
    size_t i;

    if (!mshell || !cmd || !cmd[0])
        return (1);
    if (mshell->env_last_cmd)
    {
        free(mshell->env_last_cmd);
        mshell->env_last_cmd = NULL;
    }
    i = 0;
    while (cmd[i])
        i++;
    if (i > 0 && cmd[i - 1])
    {
        mshell->env_last_cmd = ft_strdup(cmd[i - 1]);
        if (!mshell->env_last_cmd)
        {
            ft_printf_fd(2, "minishell: $_ backup failed (malloc)\n");
            mshell->exit_code = 1;
            return (1);
        }
    }
    return (0);
}


//// new

int env_add(t_shell *mshell, const char *key, const char *value)
{
    char *tmp;
    size_t i;
    size_t len;
    size_t env_len;

    if (!mshell || !key)
        return (1);
    len = ft_strlen(key);
    if (value)
        len += ft_strlen(value) + 1;
    len += 1;
    tmp = ft_calloc(1, len);
    if (!tmp)
    {
        ft_printf_fd(2, "minishell: env_add: malloc failed\n");
        mshell->exit_code = 1;
        return (1);
    }
    ft_strlcat(tmp, key, len);
    if (value)
    {
        ft_strlcat(tmp, "=", len);
        ft_strlcat(tmp, value, len);
    }
    i = 0;
    if (env_find_variable(mshell, key, &i))
    {
        free(mshell->envp[i]);
        mshell->envp[i] = tmp;

        if (ft_strncmp(tmp, "_=", 2) == 0) 
            env_swap_last(mshell->envp);

        return (0);
    }
    env_len = 0;
    while (mshell->envp[env_len])
        env_len++;
    mshell->envp = realloc_env(mshell->envp, env_len);
    if (!mshell->envp)
    {
        free(tmp);
        ft_printf_fd(2, "minishell: env_add: realloc failed\n");
        mshell->exit_code = 1;
        return (1);
    }
    mshell->envp[env_len] = tmp;
    mshell->envp[env_len + 1] = NULL;

    if (ft_strncmp(tmp, "_=", 2) == 0)
        env_swap_last(mshell->envp);

    return (0);
}

