/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/06/12 18:18:31 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

char	*expand_token_value(char *str, t_shell *mshell)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			handle_dollar_sign(str, mshell, &result, &i);
		else
		{
			result = char_join_result_and_free(&result, str[i++]);
		}
		if (!result)
		{
			print_error("strjoin failed");
			return (NULL);
		}
	}
	return (result);
}

void	handle_dollar_sign(char *str, t_shell *mshell, char **result, int *i)
{
	if (str[*i + 1] == '?')
		*result = expand_exit_code(mshell, *result, i);
	else if (str[*i + 1] == '_' || ft_isalpha(str[*i + 1]))
		*result = handle_env_variable(&str, mshell, i, *result);
	else if (ft_isdigit(str[*i + 1]))
	{
		*i += 2;
		*result = char_join_result_and_free(result, str[(*i)++]);
	}
	else
		*result = char_join_result_and_free(result, str[(*i)++]);
}

char	*expand_exit_code(t_shell *mshell, char *result, int *i)
{
	char	*exit_code_str;

	exit_code_str = ft_itoa(mshell->exit_code);
	if (!exit_code_str)
	{
		print_error("ft_itoa memory allocation failed");
		return (NULL);
	}
	result = str_join_result_and_free(&result, exit_code_str);
	if (!result)
	{
		print_error("strjoin failed");
		return (NULL);
	}
	free_string(exit_code_str);
	*i = *i + 2;
	return (result);
}

char	*handle_env_variable(char **str, t_shell *mshell, int *i, char *result)
{
	char	*var_name;
	char	*env_value;

	(*i)++;
	var_name = extract_variable_name(str, &var_name, i, &result);
	if (!var_name)
		return (NULL);
	else if (var_name[0] == '\0')
		return (result);
	env_value = env_find_value(mshell, var_name);
	if (!env_value)
		env_value = "";
	result = str_join_result_and_free(&result, env_value);
	free_string(var_name);
	if (!result)
		return (NULL);
	return (result);
}

char	*extract_variable_name(char **str, char **var_name, int *i,
			char **result)
{
	int		start;

	start = *i;
	if (!ft_isalpha((*str)[*i]) && (*str)[*i] != '_')
	{
		*var_name = ft_strdup("");
		if (!*var_name)
		{
			print_error("Strdup failed");
			return (NULL);
		}
		return (*var_name);
	}
	(*i)++;
	while (ft_isalnum((*str)[*i]) || (*str)[*i] == '_')
		(*i)++;
	*var_name = ft_substr(*str, start, (*i) - start);
	if (!*var_name)
	{
		print_error("Substr failed in handle_env_variables");
		return (NULL);
	}
	else if (ft_strcmp(*var_name, "EMPTY") == 0)
	{
		free_string(*result);
		*result = ft_strdup("");
		if (!*result)
			return (NULL);
		free_string(*var_name);
		return (*result);
	}
	return (*var_name);
}
