/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 05:28:39 by trpham            #+#    #+#             */
/*   Updated: 2025/06/03 14:28:32 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	is_redirection(t_token *token)
{
	if (token->type == REDIR_IN_TOKEN || token->type == REDIR_OUT_TOKEN
		|| token->type == REDIR_HEREDOC_TOKEN
		|| token->type == REDIR_APPEND_TOKEN)
		return (TRUE);
	return (FALSE);
}

/*
	Redirection must be followed by word;
	IN or HEREDOC can be followed by here-doc delimiter.
*/

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
