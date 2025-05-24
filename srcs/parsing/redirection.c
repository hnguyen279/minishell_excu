/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 05:28:39 by trpham            #+#    #+#             */
/*   Updated: 2025/05/16 06:26:36 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	is_redirection(t_token *token)
{
	if (token->type == REDIR_IN_token || token->type == REDIR_OUT_token
		|| token->type == REDIR_HEREDOC_token
		|| token->type == REDIR_APPEND_token)
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

	current = token_list;
	if (!current)
		return (FALSE);
	if (is_redirection(current) == TRUE)
	{
		get_error_msg(ERR_REDIR);
		return (FALSE);
	}
	while (current)
	{
		if (is_redirection(current) == TRUE)
		{
			if (!current->next || current->next->type != WORD)
			{
				get_error_msg(ERR_REDIR);
				return (FALSE);
			}
		}
		current = current->next;
	}
	return (TRUE);
}
