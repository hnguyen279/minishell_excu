/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 16:13:40 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

char	*expand_token_value(char *str, t_shell *mshell)
{
	int		i;
	char	*result;
	char	*exit_code_str;
	char	*tmp;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			if (str[i + 1] == '?')
			{
				exit_code_str = ft_itoa(mshell->exit_code);
				if (!exit_code_str)
				{
					print_error("ft_itoa memory allocation failed");
					return (NULL);
				}
				result = str_join_result_and_free(&result, exit_code_str);
				free_string(exit_code_str);
				i = i + 2;
			}
			else if (str[i + 1] == '_' || ft_isalpha(str[i + 1]))
			{
				i++;
				tmp = result;
				result = handle_env_variable(&str, mshell, &i, result);
				free_string(tmp);
	
			}
			else
			{
				result = char_join_result_and_free(&result, str[i]);
				i++;
			}
		}
		else
		{
			result = char_join_result_and_free(&result, str[i]);
			i++;
		}
		if (!result)
		{
			print_error("strjoin failed");
			return (NULL);
		}
	}
	return (result);
}



char	*handle_env_variable(char **str, t_shell *mshell, int *i, char *result)
{
	char	*var_name;
	int		start;
	char	*env_value;
	
	start = *i;
	while (ft_isalpha((*str)[*i]) || (*str)[*i] == '_')
		(*i)++;
	var_name = ft_substr(*str, start, (*i) - start);
	if (!var_name)
	{
		print_error("Substr failed in handle_env_variables");
		return (NULL);
	}
	else if (ft_strcmp(var_name, "EMPTY") == 0)
	{
		result = ft_strdup("");
		if (!result)
			return (NULL);
		free_string(var_name);
		return (result);
	}
	env_value = env_find_value(mshell, var_name);
	if (!env_value)
		env_value = "";
	result = str_join_result_and_free(&result, env_value);
	free_string(var_name);
	if (!result)
		return (NULL);
	return (result);	
}

char	*str_join_result_and_free(char **s1, char *s2)
{
	char	*joined_str;

	joined_str = ft_strjoin(*s1, s2);
	free_string(*s1);
	if (!joined_str)
	{
		print_error("ft_strjoin failed");
		return (NULL);
	}
	return (joined_str);
}

char	*char_join_result_and_free(char **s1, char c)
{
	char *joined_str;
	char *s2;

	s2 = calloc(2, 1);
	s2[0] = c;
	joined_str = str_join_result_and_free(s1, s2);
	// free_string(s1);
	free_string(s2);
	return (joined_str);
}
