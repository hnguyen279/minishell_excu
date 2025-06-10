/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:27:47 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 19:13:48 by trpham           ###   ########.fr       */
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
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->ori_value = ft_strdup(ori_s);
	if (!new_token->ori_value)
	{
		free_string(new_token->value);
		free(new_token);
		return (NULL);
	}
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
		*tokenized_input_list = new_token;
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
	// print_linked_list(temp);
	count = 0;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == WORD)
			count++;
		temp = temp->next;
	}
	return (count);
}

char *extract_ori_word(char *line, int *i)
{
	int		start;
	int		index;
	char	*word;

	start = *i;
	index = *i;
	
	if (line[index] == '\'' || line[index] == '"')
	{
		word = extract_original_value_with_quote(line, &index);
	}
	else
	{
		word = extract_original_without_quote(line, &index);
	}
	if (!word)
		return (NULL);
	return (word);
}

char	*extract_original_value_with_quote(char *line, int *index)
{
	char	quote;
	char	*res;
	int		start;
	
	quote = line[*index];
	res = NULL;
	start = *index;
	(*index)++;
	while (line[*index] && line[*index] != quote)
	{
		(*index)++;
	}
	if (!line[*index])
	{
		print_error("Unclosed quote");
		return (NULL);
	}
	res = ft_substr(line, start, *index);
	if (!res)
		return (NULL);
	return (res);
}

char	*extract_original_without_quote(char *line, int *index)
{
	char	*res;
	int		start;
	
	res = NULL;
	start = *index;
	while (line[*index] && ft_isspace(line[*index]) == FALSE && ft_isspecial(line[*index]) == FALSE)
	{
		(*index)++;
	}
	res = ft_substr(line, start, *index - start);
	if (!res)
		return (NULL);
	return (res);
}
