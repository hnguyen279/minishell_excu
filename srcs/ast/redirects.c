/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 05:28:39 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 12:01:59 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// remove !*token_list condition,
//	cause that's when the token_list reaching the end, not an error
int	parse_redirection(t_cmd **new_cmd, t_token **token_list)
{
	t_redirect_type	redir_type;

	if (check_redir_type_before_parsing(new_cmd, token_list,
			&redir_type) == FALSE)
		return (FALSE);
	(*token_list) = (*token_list)->next;
	if (!*token_list || (*token_list)->type != WORD || !(*token_list)->value)
	{
		print_error("Invalid or missing file after redirection");
		return (FALSE);
	}
	if (add_redirects(&(*new_cmd)->redirects, redir_type, token_list) == FALSE)
		return (FALSE);
	(*token_list) = (*token_list)->next;
	return (TRUE);
}

int	add_redirects(t_redirect **redir_list, t_redirect_type type,
		t_token **token_list)
{
	t_redirect	*new_redir;
	t_redirect	*current;

	new_redir = NULL;
	current = NULL;
	if (!(*token_list) || !(*token_list)->value || !(*token_list)->ori_value)
	{
		print_error("Empty file name in redirection");
		return (FALSE);
	}
	if (create_redirect(&new_redir, token_list, type) == FALSE)
		return (FALSE);
	if (!*redir_list)
		*redir_list = new_redir;
	else
	{
		current = *redir_list;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (TRUE);
}

int	create_redirect(t_redirect **new_redir, t_token **token_list,
			t_redirect_type type)
{
	if (check_ambiguous_redirect(token_list) == FALSE)
	{
		print_error("Ambiguous redirect");
		return (FALSE);
	}
	*new_redir = malloc(sizeof(t_redirect));
	if (!*new_redir)
	{
		print_error("Failed to add redirects");
		return (FALSE);
	}
	if (add_value_to_redirect(new_redir, token_list, type) == FALSE)
	{
		free(*new_redir);
		return (FALSE);
	}
	return (TRUE);
}

int	add_value_to_redirect(t_redirect **new_redir, t_token **token_list,
		t_redirect_type type)
{
	(*new_redir)->ori_file = ft_strdup((*token_list)->ori_value);
	if (!(*new_redir)->ori_file)
	{
		free(*new_redir);
		print_error("Failed to strdup file");
		return (FALSE);
	}
	(*new_redir)->tmp_file = NULL;
	(*new_redir)->next = NULL;
	(*new_redir)->file = ft_strdup((*token_list)->value);
	if (!(*new_redir)->file)
	{
		free(*new_redir);
		free_string((*new_redir)->ori_file);
		print_error("Failed to strdup file");
		return (FALSE);
	}
	(*new_redir)->type = type;
	if (type == REDIR_IN)
		(*new_redir)->fd = 0;
	else if (type == REDIR_HEREDOC)
		(*new_redir)->fd = 1;
	return (TRUE);
}
