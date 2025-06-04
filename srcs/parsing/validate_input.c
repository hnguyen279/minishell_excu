/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:27 by trpham            #+#    #+#             */
/*   Updated: 2025/06/04 10:33:59 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	validate_token(t_token *token_list)
{
	// no pipe at start, end and double pipes
	if (is_valid_pipe(token_list) == FALSE)
		return (FALSE);
	// no redirection at start, end or without filename
	
	if (is_valid_redirection(token_list) == FALSE)
		return (FALSE);
	
	// check valid parentheses necessary or not ?
	if (is_valid_parentheses(token_list) == FALSE)
		return (FALSE);

		
	return (TRUE);
}

int	is_valid_parentheses(t_token *token_list)
{
	// t_token	*temp;

	// if (!token_list)
	// 	return (FALSE);
	// temp = token_list;
	// while (temp)
	// {
	// 	if (validate_parentheses_pair(temp->value) == FALSE)
	// 		return (FALSE);
	// }
	(void) token_list;
	
	
	return (TRUE);
}


/*
Read character by character and check the quote here
*/
int	validate_quote(char *line)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		print_error("Unclosed quote");
		return (FALSE);
	}
	return (TRUE);
}

int	validate_parentheses_pair(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '(')
			count++;
		else if (line[i] == ')')
		{
			count--;
			if (count < 0)
			{
				print_error("Unmatched closing parenthesis");
				return (FALSE);
			}
		}
		i++;
	}
	if (count != 0)
	{
		print_error("Unclosed parenthesis");
		return (FALSE);
	}
	return (TRUE);
}

int	is_valid_pipe(t_token *token)
{
	t_token	*temp;

	if (!token)
		return (FALSE);
	
	temp = token;
	if (temp->type == PIPE)
	{
		print_error("Unexpected token '|'");
		return (FALSE);
	}
	while (temp)
	{
		if (temp->type == PIPE && temp->next == NULL)
		{
			// get_error_msg(ERR_PIPE);
			print_error("Unexpected token '|'");
			return (FALSE);
		}
		// if (temp->type == PIPE && (temp->next->type == PIPE))
		// {
		// 	// get_error_msg(ERR_PIPE);
		// 	print_error("Unexpected token '|'");
		// 	return (FALSE);
		// }
		temp = temp->next;
	}
	return (TRUE);
}

int	is_pipe(t_token *token)
{
	if (token->type == PIPE)
		return (TRUE);
	return (FALSE);
}

int	is_operator(t_token *token)
{
	if (is_pipe(token) == TRUE || is_redirection(token) == TRUE)
		return (TRUE);
	return (FALSE);
}

// int	is_comment(char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '#')
// 			return (TRUE);
// 		i++;
// 	}
// 	return (FALSE);
// }
