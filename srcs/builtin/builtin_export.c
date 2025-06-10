/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:50:07 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/10 12:19:09 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	handle_plus_equal_case(t_shell *mshell, const char *arg,
		char *plus_equal)
{
	char	*key;

	key = ft_substr(arg, 0, plus_equal - arg);
	if (!key || !export_is_valid_key(key))
	{
		free(key);
		ft_printf_fd(2, "minishell: export: `%s`: not a valid identifier\n", arg);
		return (1);
	}
	free(key);
	return (export_plus_equal(mshell, arg, plus_equal));
}

static int	export_handle_one(t_shell *mshell, const char *arg)
{
	char	*equal;
	char	*plus_equal;

	if (!arg)
		return (0);
	if (arg[0] == '-')
	{
		ft_printf_fd(2, "minishell: export: `%s`: invalid option\n", arg);
		return (2);
	}
	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal != NULL)
		return (handle_plus_equal_case(mshell, arg, plus_equal));
	if (!export_is_valid_key(arg))
	{
		ft_printf_fd(2, "minishell: export: `%s`: not a valid identifier\n", arg);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (equal != NULL)
		return (export_with_equal(mshell, arg, equal));
	else if (env_add(mshell, arg, NULL) != 0)
		return (1);
	return (0);
}

static int	handle_export_arguments(t_shell *mshell, char **token, int argc)
{
	int	i;
	int	code;
	int	result;

	i = 1;
	code = 0;
	while (i < argc)
	{
		result = export_handle_one(mshell, token[i]);
		if (result > code)
			code = result;
		i++;
	}
	return (code);
}

static int	export_standalone(char **envp)
{
	size_t	count;
	size_t	i;
	char	**copy;

	count = 0;
	i = 0;
	copy = copy_and_sort_envp(envp, &count);
	if (!copy)
		return (1);
	while (i < count)
	{
		if (copy[i][0] != '_' || (copy[i][1] != '\0' && copy[i][1] != '='))
			export_print(copy[i]);
		i++;
	}
	free(copy);
	return (0);
}

int	builtin_export(t_shell *mshell, char **token)
{
	int	argc;

	if (!mshell || !token || !token[0])
	{
		ft_printf_fd(2, "minishell: export: invalid input\n");
		return (1);
	}
	argc = 0;
	while (token[argc])
		argc++;
	if (argc == 1)
		return (export_standalone(mshell->envp));
	else
		return (handle_export_arguments(mshell, token, argc));
}
