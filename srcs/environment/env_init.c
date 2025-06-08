/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:04:38 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 16:24:47 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			free_array_null(&res);
			return (NULL);
		}
		i++;
	}
	free_array_null(&envp);
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
			return (free_array_null(&res), NULL);
		i++;
	}
	return (res);
}
