/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:31:59 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 15:31:59 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	export_is_valid_key(const char *str)
{
	size_t	i;

	if (!str || str[0] == '\0' || str[0] == '=' || ft_isdigit(str[0])
		|| str[0] == '+')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	export_print(const char *env)
{
	const char	*equal;

	equal = ft_strchr(env, '=');
	if (equal)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, env, equal - env + 1);
		ft_printf_fd(STDOUT_FILENO, "\"%s\"\n", equal + 1);
	}
	else
	{
		ft_printf_fd(STDOUT_FILENO, "declare -x %s\n", env);
	}
}

char	**copy_and_sort_envp(char **envp, size_t *out_count)
{
	size_t	i;
	char	**copy;

	i = 0;
	*out_count = 0;
	while (envp[*out_count])
		(*out_count)++;
	copy = ft_calloc(*out_count + 1, sizeof(char *));
	if (!copy)
	{
		ft_printf_fd(2, "minishell: export: malloc failed\n");
		return (NULL);
	}
	while (i < *out_count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_split(copy);
			return (NULL);
		}
		i++;
	}
	env_sort(copy, *out_count);
	return (copy);
}

static char	*build_plus_equal_value(t_shell *mshell, const char *key,
		const char *plus_equal)
{
	char	*old_val;
	char	*new_val;

	old_val = env_find_value(mshell, key);
	if (old_val)
		new_val = ft_strjoin(old_val, plus_equal + 2);
	else
		new_val = ft_strdup(plus_equal + 2);
	return (new_val);
}

int	export_plus_equal(t_shell *mshell, const char *arg,
		char *plus_equal)
{
	char	*key;
	char	*new_val;

	key = ft_substr(arg, 0, plus_equal - arg);
	if (!key)
		return (1);
	new_val = build_plus_equal_value(mshell, key, plus_equal);
	if (!new_val)
	{
		free(key);
		ft_printf_fd(2, "minishell: export: malloc failed\n");
		return (1);
	}
	env_remove(mshell, key);
	if (env_add(mshell, key, new_val) != 0)
	{
		free(key);
		free(new_val);
		return (1);
	}
	free(key);
	free(new_val);
	return (0);
}
