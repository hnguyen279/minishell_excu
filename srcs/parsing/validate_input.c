/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:27 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 22:16:27 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
Read character by character and check the quote here
*/

// H add for error msg
void	print_token_error(t_token *token)
{
	const char	*str;

	if (!token)
		str = "newline";
	else if (token->type == REDIR_IN_TOKEN)
		str = "<";
	else if (token->type == REDIR_OUT_TOKEN)
		str = ">";
	else if (token->type == REDIR_APPEND_TOKEN)
		str = ">>";
	else if (token->type == REDIR_HEREDOC_TOKEN)
		str = "<<";
	else if (token->type == PIPE)
		str = "|";
	ft_printf_fd(2, "Error: syntax error near unexpected token `%s'\n",
		str);
}

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

int	validate_token(t_token *token_list)
{
	if (is_valid_pipe(token_list) == FALSE)
		return (FALSE);
	if (is_valid_redirection(token_list) == FALSE)
		return (FALSE);
	return (TRUE);
}

int	is_valid_pipe(t_token *token)
{
	t_token	*temp;

	if (!token)
	{
		// printf("broke here\n");
		return (FALSE);
	}
	temp = token;
	if (temp->type == PIPE)
	{
		//print_error("Unexpected token '|'");
		print_token_error(temp);
		return (FALSE);
	}
	while (temp)
	{
		if (temp->type == PIPE && temp->next == NULL)
		{
			//print_error("Unexpected token '|'");
			print_token_error(temp);
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
				//print_error("Syntax error near redirection");
				print_token_error(current->next);
				return (FALSE);
			}
			current = current->next;
		}
		current = current->next;
	}
	return (TRUE);
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
