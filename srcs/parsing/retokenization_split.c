/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenization_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:08:33 by trpham            #+#    #+#             */
/*   Updated: 2025/06/11 19:10:07 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"


int	retokenizer(t_token **token_list)
{
	t_token	*current;
	t_token	*next_token;
	t_token	*prev_token;

	current = *token_list;
	prev_token = NULL;
	next_token = NULL;
	while (current)
	{
		next_token = current->next;
		if (current->type == WORD && ft_strchr(current->value, ' ')
			&& (current->ori_value) && *(current->ori_value) == '$') //recheck this condition
		{
			if (link_split_token(&current, &prev_token, &next_token, token_list) == FALSE)
			{
				return (FALSE);
			}
			current = current->next;
		}
		else
		{
			prev_token = current;
			current = current->next;
		}
	}
	return (TRUE);
}

int	link_split_token(t_token **current, t_token **prev_token,
		t_token **next_token, t_token **token_list)
{
	t_token	*temp;
	char	**arr_word;
	t_token	*new_last;
	
	arr_word = ft_split((*current)->value, ' ');
	if (!arr_word)
		return (FALSE);
	temp = replace_token_with_new_arr(*current, arr_word);
	if (!temp)
		return (FALSE);
	if (!*prev_token)
		*token_list = temp;
	else
		(*prev_token)->next = temp;
	new_last = temp;
	while (new_last->next)
		new_last = new_last->next;
	new_last->next = *next_token;
	*prev_token = new_last;
	// free_token_list(temp);
	return (TRUE);
}

t_token	*replace_token_with_new_arr(t_token *current, char **arr)
{
	t_token	*new_head = NULL;
	t_token	*prev = NULL;
	t_token	*new_token = NULL;
	int		i;
	
	i = 0;
	while (arr[i])
	{
		new_token = create_token(arr[i], current->ori_value, WORD);
		if (!new_token)
			return (NULL);
		if (!prev)
		{
			new_head = new_token;
			prev = new_head;
		}
		else
		{
			prev->next = new_token;
			prev = prev->next;
		}
		i++;
	}
	free_array(arr, i);
	// printf("----------------------------\n"); //debug
	// printf("The split token list\n"); //debug
	// // print_linked_list(new_head); //debug
	return (new_head);
}
