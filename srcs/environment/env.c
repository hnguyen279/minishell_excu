
#include "minishell.h"

int init_env(t_shell *mshell, char **envp)
{
    size_t len;
    size_t i;

    if (!mshell || !envp)
        return (1);
    len = 0;
    while (envp[len])
        len++;
    mshell->envp = ft_calloc(len + 1, sizeof(char *));
    if (!mshell->envp)
        return (ft_printf_fd(2, "minishell: init_env: malloc failed\n"), 1);
    i = 0;
    while (i < len)
    {
        mshell->envp[i] = ft_strdup(envp[i]);
        if (!mshell->envp[i])
            return (env_free(mshell), ft_printf_fd(2, "minishell: init_env: malloc failed\n"), 1);
        i++;
    }
    return (0);
}

void env_free(t_shell *mshell)
{
    size_t i;

    if (!mshell || !mshell->env)
        return;
    i = 0;
    while (mshell->env[i])
    {
        free(mshell->env[i]);
        mshell->env[i] = NULL;
        i++;
    }
    free(mshell->env);
    mshell->env = NULL;
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
            return (ft_multi_free_null(&res), NULL);
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

static char *env_find_variable(t_shell *mshell, const char *key, size_t *i) //found key and value
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

const char *env_find_value(t_shell *mshell, const char *key) // found value
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
        res[i] = envp[i];
        i++;
    }
    free(envp);
    return (res);
}

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
        len += ft_strlen(value) + 1; // + 1 for '='
    len += 1; //for null terminal
    tmp = ft_calloc(1, len);
    if (!tmp)
        return (ft_printf_fd(2, "minishell: env_add: malloc failed\n"), 1);
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
        env_swap_last(mshell->envp);
        return (0);
    }
    env_len = 0;
    while (mshell->envp[env_len])
        env_len++;
    mshell->envp = realloc_env(mshell->envp, env_len);
    if (!mshell->envp)
        return (free(tmp), ft_printf_fd(2, "minishell: env_add: malloc failed\n"), 1);
    mshell->envp[env_len] = tmp;
    mshell->envp[env_len + 1] = NULL;
    env_swap_last(mshell->envp);
    return (0);
}


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
    return (0);
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

static void env_swap_last(char **envp)
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

int env_underscore(t_shell *mshell, char **cmd)
{
    char *last_cmd;
    size_t i;

    if (!mshell || !cmd)
        return (1);
    i = 0;
    while (cmd[i])
        i++;
    if (i > 0 && !mshell->tokens->has_pipe)
    {
        last_cmd = cmd[i - 1];
        return (env_add(mshell, "_", last_cmd));
    }
    return (1);
}

int env_clone_underscore(t_shell *mshell)
{
    const char *underscore_val;

    if (!mshell || !mshell->tokens)
        return (1);
    underscore_val = env_find_value(mshell, "_");
    if (underscore_val && *underscore_val)
    {
        mshell->tokens->env_last_cmd = ft_strdup(underscore_val);
        if (!mshell->tokens->env_last_cmd)
            return (ft_printf_fd(2, "minishell: env_clone_underscore: malloc failed\n"), 1);
    }
    return (0);
}