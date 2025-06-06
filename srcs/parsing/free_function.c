/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:29:33 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 16:14:20 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"


void	free_string(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void	free_array(char **arr, int i)
{
	int	j;

	j = 0;
	if (!arr)
		return ;
	while (j < i)
	{
		free_string(arr[j]);
		j++;
	}
	free(arr);
}

void	free_token_list(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
	tokens = NULL;
}

void	free_cmd_list(t_cmd *head)
{
	t_cmd *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		// free(temp->args);
		free(temp->cmd_name);
		free(temp);
	}
	head = NULL;
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
	free_string(s2);
	return (joined_str);
}