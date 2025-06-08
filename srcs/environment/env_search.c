/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_search.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:05:10 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 16:05:11 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static size_t	strlen_until_char(const char *s, const char ch)
{
	size_t	len;

	len = 0;
	while (*s && *s != ch)
	{
		len++;
		s++;
	}
	return (len);
}

char	*env_find_variable(t_shell *mshell, const char *key, size_t *i)
{
	size_t	key_len;
	size_t	env_key_len;

	if (!mshell || !mshell->envp || !key || !i)
		return (NULL);
	key_len = ft_strlen(key);
	*i = 0;
	while (mshell->envp[*i])
	{
		env_key_len = strlen_until_char(mshell->envp[*i], '=');
		if (key_len == env_key_len && ft_strncmp(mshell->envp[*i], key,
				key_len) == 0)
			return (mshell->envp[*i]);
		(*i)++;
	}
	return (NULL);
}

char	*env_find_value(t_shell *mshell, const char *key)
{
	size_t	i;
	size_t	key_len;
	size_t	env_key_len;

	if (!mshell || !mshell->envp || !key)
		return (NULL);
	if (*key == '$')
		key++;
	key_len = ft_strlen(key);
	i = 0;
	while (mshell->envp[i])
	{
		env_key_len = strlen_until_char(mshell->envp[i], '=');
		if (key_len == env_key_len && ft_strncmp(mshell->envp[i], key,
				key_len) == 0)
			return (mshell->envp[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
