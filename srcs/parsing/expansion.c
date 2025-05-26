/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/05/26 18:46:43 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

void	expand_variables(t_token **token_list, t_shell *mshell)
{
	t_token	*temp;
	char	*expanded_value;

	temp = *token_list;
	while (temp)
	{
		if (temp->type == WORD) // adjust: only expand inside double quotes
		{
			expanded_value = expand_token_value(temp->value, mshell);
			if (expanded_value)
			{
				free_string(temp->value);
				temp->value = ft_strdup(expanded_value);
				if (!temp->value)
				{
					free_string(expanded_value);
					return ;
				}
			}
			temp = temp->next;
		}
	}
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
				if (!result)
					return (NULL);
				i = i + 2;
			}
			// else if (str[i + 1] == '_' || ft_isalpha(str[i + 1]))
			// {
			// }
		}
		else
		{
			result = char_join_result_and_free(result, str[i]);
			printf("after join: %s\n", result);
			if (!result)
				return (NULL);
			i++;
		}
	}
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