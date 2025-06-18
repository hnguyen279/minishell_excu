/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:27 by trpham            #+#    #+#             */
/*   Updated: 2025/06/18 15:15:23 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
Read character by character and check the quote here
*/

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
	ft_printf_fd(2, "minishell: syntax error near unexpected token `%s'\n",
		str);
}

int	validate_quote(char *line)
{
	int		i;
	int		quote;
	char	mark_quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && quote == 0)
		{
			mark_quote = line[i];
			quote = line[i];
		}
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		ft_printf_fd(2, "minishell: unexpected EOF while looking for "
			"matching `%c'\n minishell: syntax error: "
			"unexpected end of file\n", mark_quote);
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
		return (FALSE);
	temp = token;
	if (temp->type == PIPE)
	{
		print_token_error(temp);
		return (FALSE);
	}
	while (temp)
	{
		if (temp->type == PIPE && temp->next == NULL)
		{
			print_token_error(temp);
			return (FALSE);
		}
		if (temp->type == PIPE && (temp->next->type == PIPE))
		{
			print_token_error(temp);
			return (FALSE);
		}
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
			if (!current->next)
			{
				print_token_error(current->next);
				return (FALSE);
			}
			if (current->next && current->next->type != WORD)
			{
				print_token_error(current->next);
				return (FALSE);
			}
			// current = current->next;
		}
		current = current->next;
	}
	return (TRUE);
}
