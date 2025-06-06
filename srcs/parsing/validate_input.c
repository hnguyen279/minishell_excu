/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:27 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 15:32:41 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	validate_token(t_token *token_list)
{
	if (is_valid_pipe(token_list) == FALSE)
		return (FALSE);
	if (is_valid_redirection(token_list) == FALSE)
		return (FALSE);
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

// int	validate_parentheses_pair(char *line)
// {
// 	int	i;
// 	int	count;

// 	i = 0;
// 	count = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '(')
// 			count++;
// 		else if (line[i] == ')')
// 		{
// 			count--;
// 			if (count < 0)
// 			{
// 				print_error("Unmatched closing parenthesis");
// 				return (FALSE);
// 			}
// 		}
// 		i++;
// 	}
// 	if (count != 0)
// 	{
// 		print_error("Unclosed parenthesis");
// 		return (FALSE);
// 	}
// 	return (TRUE);
// }

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


/*
	Redirection must be followed by word;
	IN or HEREDOC can be followed by here-doc delimiter.
*/
int	is_redirection(t_token *token)
{
	if (token->type == REDIR_IN_TOKEN || token->type == REDIR_OUT_TOKEN
		|| token->type == REDIR_HEREDOC_TOKEN
		|| token->type == REDIR_APPEND_TOKEN)
		return (TRUE);
	return (FALSE);
}

int	is_valid_redirection(t_token *token_list)
{
	t_token	*current;

	if (!token_list)
		return (FALSE);
	current = token_list;	
	while (current)
	{
		if (is_redirection(current) == TRUE)
		{
			if (!current->next || current->next->type != WORD)
			{
				print_error("Invalid after redirection");
				return (FALSE);
			}
			current = current->next; //H add
		}
		current = current->next;
	}
	return (TRUE);
}