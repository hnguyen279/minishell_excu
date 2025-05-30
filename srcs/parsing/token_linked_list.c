/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_linked_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:27:47 by trpham            #+#    #+#             */
/*   Updated: 2025/05/28 07:42:29 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_token	*create_token(char *s, t_token_type i)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(s);
	if (!new_token->value)
		return (NULL);
	new_token->type = i;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->in_single_quote = FALSE;
	return (new_token);
}

void	add_token(t_token **tokenized_input_list, t_token *new_token)
{
	t_token	*temp;

	if (!new_token)
	{
		print_error("Invalid token");
		// get_error_msg(ERR_MALLOC);
		return ;
	}
	if (*tokenized_input_list == NULL)
	{
		*tokenized_input_list = new_token;
		// printf("%s\n", tokenized_input_list->value);
	}
	else
	{
		temp = *tokenized_input_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
		// printf("%s\n", tokenized_input_list->value);
	}
}
