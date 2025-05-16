/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:27 by trpham            #+#    #+#             */
/*   Updated: 2025/05/13 07:06:09 by trpham           ###   ########.fr       */
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
	{
		quote = line[i];
	}
	else if (line[i] == quote)
		quote = 0;
	i++;
}
if (quote != 0)
{
	get_error_msg(ERR_QUOTE);
	return (-1);
}

return (0);	
}

int	is_valid_pipe(t_token *token)
{
t_token	*temp;

temp = token;
if (temp->type == PIPE)
{
	get_error_msg(ERR_PIPE);
	return (FALSE);
}
while (temp)
{
	if (temp->type == PIPE && temp->next == NULL)
	{
		get_error_msg(ERR_PIPE);
		return (FALSE);
	}
	if (temp->type == PIPE && (temp->next->type == PIPE ||
		is_redirection(temp->next) == TRUE))
	{
		get_error_msg(ERR_PIPE);
		return (FALSE);
	}
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

// check the remaining function of the file, may not necessary
// int	is_valid_quote(t_token *token)
// {
// 	t_token	*temp;

// 	temp = token;
// 	while (temp)
// 	{
// 		if (check_double_quote(temp->value) == FALSE)
// 		{
// 			return (FALSE);
// 		}
// 		temp = temp->next;
// 	}
// 	return (TRUE);
// }

// int	check_double_quote(char *str)
// {
// 	int	i;
// 	// char	open_quote;
// 	char	current_quote;
	
// 	if (!str)
// 		return (FALSE);
// 	// open_quote = '0';
// 	current_quote = '0';
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			if (current_quote == '0')
// 			{
// 				current_quote = str[i];
// 				// open_quote = current_quote;
// 			}
// 			else if (current_quote == str[i])
// 				current_quote = '0';
// 			// else if (open_quote != str[i])
// 			// 	open_quote = '1';
// 		}
// 		i++;
// 	}
// 	if (current_quote != '0')
// 	{
// 		get_error_msg(ERR_QUOTE);
// 		return (FALSE);
// 	}
// 	// else if (open_quote == '1')
// 	// {
// 	// 	print_syntax_error("Invalid syntax\n");
// 	// 	return (FALSE);
// 	// }
// 	return (TRUE);
// }
