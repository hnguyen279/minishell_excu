#include "../../includes/shell.h"

char	**realloc_env(char **envp, size_t len)
{
	char	**res;
	size_t	i;

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

char	**env_dup(char **envp)
{
	char	**res;
	size_t	i;
	size_t	len;

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
