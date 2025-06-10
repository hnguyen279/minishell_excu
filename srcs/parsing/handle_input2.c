/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 15:34:40 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 15:30:52 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	skip_expanded_empty_var(t_token **token_list)
{
	if (ft_strcmp((*token_list)->ori_value, "\"\"")
		&& ft_strcmp((*token_list)->ori_value, "\'\'"))
	{
		skip_first_empty_vars(token_list);
		// skip_middle_empty_vars(token_list);
	}
	return (TRUE);
}

void	skip_first_empty_vars(t_token **token_list)
{
	t_token	*to_free;

	while (*token_list && ft_strcmp((*token_list)->value, "") == 0)
	{
		to_free = *token_list;
		(*token_list) = (*token_list)->next;
		free_string(to_free->value);
		free_string(to_free->ori_value);
		free(to_free);
	}
}

// void	skip_middle_empty_vars(t_token **token_list)
// {
// 	t_token	*to_free;
// 	t_token	*temp;
// 	t_token	*prev;

// 	temp = *token_list;
// 	prev = NULL;
// 	while (temp)
// 	{
// 		if (ft_strcmp(temp->value, "") == 0 && temp->next)
// 		{
// 			to_free = temp;
// 			temp = temp->next;
// 			free_string(to_free->value);
// 			free_string(to_free->ori_value);
// 			free(to_free);
// 			prev->next = temp;
// 		}
// 		else
// 		{
// 			if (prev == NULL)
// 				prev = temp;
// 			else
// 				prev = prev->next;
// 			temp = temp->next;
// 		}
// 	}
// }
