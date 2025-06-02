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

size_t strlen_until_char(const char *s, const char ch)
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
