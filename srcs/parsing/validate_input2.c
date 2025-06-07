/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:28:25 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 16:16:44 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
