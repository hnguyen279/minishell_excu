/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/06/04 14:50:22 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

t_token	*expand_variables(t_token **token_list, t_shell *mshell)
{
	t_token	*temp;
	char	*expanded_value;
	t_token *prev;

	temp = *token_list;
	prev = NULL;
	while (temp)
	{
		if (temp->type == WORD && temp->in_single_quote == FALSE) // adjust: only expand inside double quotes
		{
			expanded_value = expand_token_value(temp->value, mshell);
			if (!expanded_value)
				return (NULL);
			else if (ft_strcmp(expanded_value, "") == 0) //else if (expanded_value[0] == '\0')

			{
				free_string(temp->value);
				temp->value = ft_strdup("");
				
				// t_token *to_free = temp;
				// temp = temp->next;
				// if (!prev)
					// *token_list = temp;
				// else
					// prev->next = temp;
				// free_string(to_free->value);
				// free(to_free);
				free_string(expanded_value);
				if (!temp->value)
					return (NULL);
			}
			else
			{
				free_string(temp->value);
				temp->value = ft_strdup(expanded_value);
				free_string(expanded_value); // move here avoid leak
				if (!temp->value)
					return (NULL);
				
			}
		}
		prev = temp;
		temp = temp->next;
	}
	return (*token_list);
}

char	*expand_token_value(char *str, t_shell *mshell)
{
	int		i;
	char	*result;
	char	*exit_code_str;

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
				result = str_join_result_and_free(result, exit_code_str);
				i = i + 2;
			}
			else if (str[i + 1] == '_' || ft_isalpha(str[i + 1]))
			{
				i++;
				result = handle_env_variable(&str, mshell, &i, result);
				// printf("after join: %s\n", result);
				if (ft_strcmp(result, " ") == 0)
				{
					// printf("break here\n");
					return (result);
				}
			}
			else
			{
				result = char_join_result_and_free(result, str[i]);
				i++;
			}
			// printf("after join: %s\n", result);
		}
		else
		{
			result = char_join_result_and_free(result, str[i]);
			// printf("after join: %s\n", result);
			i++;
		}
		if (!result)
			return (NULL);
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
		// printf("Result is empty\n");
		return (result);
	}
	// printf("var_name: %s\n", var_name);
	env_value = env_find_value(mshell, var_name);
	if (!env_value)
		env_value = "";
		// return (NULL);
	
	// printf("env_value %s\n", env_value);
	result = str_join_result_and_free(result, env_value);
	if (!result)
		return (NULL);
	free_string(var_name);
	// free_string(env_value);
	return (result);	
}

char	*str_join_result_and_free(char *s1, char *s2)
{
	char	*joined_str;

	joined_str = ft_strjoin(s1, s2);
	if (!joined_str)
	{
		print_error("ft_strjoin failed");
		return (NULL);
	}
	free_string(s1);
	return (joined_str);
}

char	*char_join_result_and_free(char *s1, char c)
{
	char *joined_str;
	char s2[2];

	s2[0] = c;
	s2[1] = '\0';
	joined_str = str_join_result_and_free(s1, s2);
	return (joined_str);
}


