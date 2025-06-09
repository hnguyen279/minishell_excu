/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:27:47 by trpham            #+#    #+#             */
/*   Updated: 2025/06/09 12:47:35 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_token	*create_token(char *s, char *ori_s, t_token_type i)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(s);
	new_token->ori_value = ft_strdup(ori_s);
	if (!new_token->value || !new_token->ori_value)
		return (NULL);
	new_token->type = i;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	add_token(t_token **tokenized_input_list, t_token *new_token)
{
	t_token	*temp;

	// if (!new_token)
	// {
	// 	print_error("Invalid token");
	// 	return ;
	// }
	if (*tokenized_input_list == NULL)
	{
		*tokenized_input_list = new_token;
	}
	else
	{
		temp = *tokenized_input_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

int	count_args(t_token *tokenized_input_list)
{
	t_token	*temp;
	int		count;

	temp = tokenized_input_list;
	count = 0;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == WORD)
			count++;
		temp = temp->next;
	}
	return (count);
}
