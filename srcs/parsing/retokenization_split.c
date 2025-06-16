/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenization_split.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:08:33 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 17:06:40 by trpham           ###   ########.fr       */
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
		if (check_split_token_condition(&current, &prev_token) == TRUE)
		{
			if (handle_split_token(&current, &prev_token, &next_token,
					token_list) == FALSE)
			{
				// print_error("Failed in retokenizer\n");	
				return (FALSE);
			}
		}
		else
		{
			prev_token = current;
			current = current->next;
		}
	}
	return (TRUE);
}

// int	check_split_token_condition(t_token **current, t_token **prev_token)
// {
// 	if ((*current)->type == WORD && ft_strchr((*current)->value, ' ')
// 		&& ((*current)->ori_value) && ft_strchr((*current)->ori_value, '$')
// 		&& !ft_strnstr((*current)->ori_value, "$ ", ft_strlen((*current)->ori_value))
// 		&& (!(*prev_token)
// 			|| ((*prev_token) && ((*prev_token)->type) != REDIR_HEREDOC_TOKEN)))
// 		return (TRUE);
// 	return (FALSE);
// }

/* bash doesn't perform split on quoted token */
int	check_split_token_condition(t_token **current, t_token **prev_token)
{
	if ((*current)->type != WORD)
		return (FALSE);
	if (*prev_token && (*prev_token)->type == REDIR_HEREDOC_TOKEN)
		return (FALSE);
	if ((ft_strchr((*current)->ori_value, '"') || ft_strchr((*current)->ori_value, '\''))
		&&  has_dollar_outside_quotes((*current)->ori_value) == TRUE)// echo " '$USER' "
	{
		// printf("failed 1\n"); //debug	
		return (FALSE); 
	}
	if ((*current)->ori_value && ft_strchr((*current)->ori_value, '$')
		&& ft_strchr((*current)->value, ' '))
		{
			// printf("not fail\n"); //debug	
			return (TRUE);
		}
	// printf("failed 2\n"); //debug	
	return (FALSE);
}

// echo $X"2" -->recheck condition
int	has_dollar_outside_quotes(char *str)
{
	int		i = 0;
	char	quote = '\0';

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (quote == 0)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
		}
		else if (str[i] == '$' && quote == 0)
			return (FALSE);
		i++;
	}
	return (TRUE);
}


int	handle_split_token(t_token **current, t_token **prev_token,
		t_token **next_token, t_token **token_list)
{
	t_token	*temp;

	if (link_split_token(current, prev_token, next_token, token_list) == FALSE)
	{
		// print_error("Failed in handle split token\n");	//debug
		return (FALSE);
	}
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
	t_token	*new_last;

	arr_word = ft_split((*current)->value, ' ');
	if (!arr_word)
	{
		print_error("Failed to split\n");	
		return (FALSE);
	}
	// printf("arr[0] : %s\n", arr_word[0]);
	// printf("prev token %s\n", (*prev_token)->value );
	
	// if (is_redirection(*prev_token) == TRUE && !arr_word[0])
	// {
	// 	print_error("Ambiguous redirection");
	// 	free(arr_word);
	// 	return (FALSE);
	// }
	// print_array(arr_word); //debug
	if (!arr_word[0])
	{
		free(arr_word);
		temp = create_token("",(*current)->value, WORD);
	}
	else
	{
		temp = replace_token_with_new_arr(*current, arr_word);
		if (!temp)
		{
			// printf("failed in replace token with new arr\n"); //debug
			return (FALSE);
		}
		
	}
	// print_linked_list(temp); //debug
	if (!*prev_token)
		*token_list = temp;
	else
		(*prev_token)->next = temp;
	new_last = temp;
	while (new_last->next)
		new_last = new_last->next;
	new_last->next = *next_token;
	*prev_token = new_last;
	
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
		// printf("arr[%s] : \n", arr[i]);
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
	return (new_head);
}
