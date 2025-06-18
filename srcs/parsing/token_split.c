/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:08:33 by trpham            #+#    #+#             */
/*   Updated: 2025/06/18 14:19:21 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	handle_split_token(t_token **current, t_token **prev_token,
		t_token **next_token, t_token **token_list)
{
	t_token	*temp;

	if (link_split_token(current, prev_token, next_token, token_list) == FALSE)
		return (FALSE);
	temp = *current;
	*current = (*current)->next;
	free_token(temp);
	return (TRUE);
}

int	link_split_token(t_token **current, t_token **prev_token,
		t_token **next_token, t_token **token_list)
{
	t_token	*temp;
	char	**arr_word;

	arr_word = ft_split((*current)->value, ' ');
	if (!arr_word)
	{
		print_error("Failed to split token\n");
		return (FALSE);
	}
	if (!arr_word[0])
	{
		free(arr_word);
		temp = create_token("", (*current)->value, WORD);
	}
	else
	{
		temp = replace_token_with_new_arr(*current, arr_word);
		if (!temp)
			return (FALSE);
	}
	link_token_list(&temp, prev_token, next_token, token_list);
	return (TRUE);
}

void	link_token_list(t_token **temp, t_token **prev_token,
		t_token **next_token, t_token **token_list)
{
	t_token	*new_last;

	if (!*prev_token)
		*token_list = *temp;
	else
		(*prev_token)->next = *temp;
	new_last = *temp;
	while (new_last->next)
		new_last = new_last->next;
	new_last->next = *next_token;
	*prev_token = new_last;
}

t_token	*replace_token_with_new_arr(t_token *current, char **arr)
{
	t_token	*new_head;
	t_token	*prev;
	t_token	*new_token;
	int		i;

	i = 0;
	new_head = NULL;
	prev = NULL;
	new_token = NULL;
	while (arr[i])
	{
		new_token = create_token(arr[i], current->ori_value, WORD);
		if (!new_token)
			return (NULL);
		token_list_from_arr(&new_head, &prev, &new_token);
		i++;
	}
	free_array_null(&arr);
	return (new_head);
}

void	token_list_from_arr(t_token **new_head, t_token **prev, t_token **new_token)
{
	if (!(*prev))
	{
		*new_head = *new_token;
		*prev = *new_head;
	}
	else
	{
		(*prev)->next = *new_token;
		*prev = (*prev)->next;
	}
}
