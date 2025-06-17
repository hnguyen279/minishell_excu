/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:06:17 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 13:09:40 by trpham           ###   ########.fr       */
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
				return (FALSE);
		}
		else
		{
			prev_token = current;
			current = current->next;
		}
	}
	return (TRUE);
}

/* bash doesn't perform split on quoted token */
int	check_split_token_condition(t_token **current, t_token **prev_token)
{
	if ((*current)->type != WORD)
		return (FALSE);
	if (*prev_token && is_redirection(*prev_token) == TRUE)
		return (FALSE);
	if ((ft_strchr((*current)->ori_value, '"')
			|| ft_strchr((*current)->ori_value, '\''))
		&& has_dollar_outside_quotes((*current)->ori_value) == TRUE)
		return (FALSE);
	if ((*current)->ori_value && ft_strchr((*current)->ori_value, '$')
		&& ft_strchr((*current)->value, ' '))
		return (TRUE);
	return (FALSE);
}

// echo $X"2" -->recheck condition
int	has_dollar_outside_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
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
