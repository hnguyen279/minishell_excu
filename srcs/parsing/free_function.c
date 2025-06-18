/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:29:33 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 19:45:39 by trpham           ###   ########.fr       */
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
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free_string(tokens->value);
		free_string(tokens->ori_value);
		free(tokens);
		tokens = tmp;
	}
}

void	free_cmd_list(t_cmd *head)
{
	t_cmd	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->cmd_name);
		free(temp);
	}
	// head = NULL; no need?
}

void	free_token(t_token *token)
{
	free_string(token->value);
	free_string(token->ori_value);
	free(token);
}
