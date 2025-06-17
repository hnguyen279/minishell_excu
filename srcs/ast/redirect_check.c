/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:23:14 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 12:59:28 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	check_redir_type_before_parsing(t_cmd **new_cmd, t_token **token_list,
		t_redirect_type *redir_type)
{
	if (!new_cmd || !token_list)
	{
		print_error("Pointer to new cmd and token list not exist");
		return (FALSE);
	}
	else if (!*new_cmd)
	{
		print_error("Invalid new_cmd in parse_redirection");
		return (FALSE);
	}
	*redir_type = token_to_redirect_type((*token_list)->type);
	if (*redir_type == REDIR_INVALID)
	{
		print_error("invalid redirection token type");
		return (FALSE);
	}
	return (TRUE);
}

t_redirect_type	token_to_redirect_type(t_token_type token_type)
{
	if (token_type == REDIR_IN_TOKEN)
		return (REDIR_IN);
	else if (token_type == REDIR_OUT_TOKEN)
		return (REDIR_OUT);
	else if (token_type == REDIR_APPEND_TOKEN)
		return (REDIR_APPEND);
	else if (token_type == REDIR_HEREDOC_TOKEN)
		return (REDIR_HEREDOC);
	return (REDIR_INVALID);
}

int	check_ambiguous_redirect(t_token **token_list)
{
	t_token	*current;
	int		count_redir_file;
	char	*ori_value;

	if (!token_list && !*token_list && !(*token_list)->value)
		return (FALSE);
	current = *token_list;
	if (ft_strcmp((*token_list)->value, "") == 0)
		return (FALSE);
	if (is_white_spaces_cmd((*token_list)->value))
		return (FALSE);
	ori_value = current->ori_value;
	count_redir_file = 1;
	current = current->next;
	while (current && current->type != PIPE
		&& is_redirection(current) == FALSE)
	{
		if (ft_strcmp(current->ori_value, ori_value) == 0)
			count_redir_file++;
		current = current->next;
	}
	if (count_redir_file != 1)
		return (FALSE);
	return (TRUE);
}
