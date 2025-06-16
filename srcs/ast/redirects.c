/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 05:28:39 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 22:25:37 by trpham           ###   ########.fr       */
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
	(*token_list) = (*token_list)->next; // recheck if removing this one affect other
	// if (!*token_list || (*token_list)->type != WORD || !(*token_list)->value
	// 	|| !*(*token_list)->value)
	// printf("parsing after redirection %s\n", (*token_list)->value);
	if (!*token_list || (*token_list)->type != WORD || !(*token_list)->value)
	{
		print_error("Invalid or missing file after redirection");
		return (FALSE);
	}
	if (add_redirects(&(*new_cmd)->redirects, redir_type,
			(token_list)) == FALSE)
	{
		// print_error("Failed to add redirects");
		return (FALSE);
	}
	(*token_list) = (*token_list)->next;
	return (TRUE);
}

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
	if (check_ambiguous_redirect(token_list) == FALSE) //added
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
	(*new_redir)->ori_file = ft_strdup((*token_list)->ori_value);
	// printf("redirect-> ori_value %s\n", (*token_list)->ori_value); //debug
	if (!(*new_redir)->ori_file)
	{
		free((*new_redir));
		print_error("Failed to strdup file");
		return (FALSE);
	}
	(*new_redir)->tmp_file = NULL;
	(*new_redir)->next = NULL;
	(*new_redir)->file = ft_strdup((*token_list)->value);
	if (!(*new_redir)->file)
	{
		free((*new_redir));
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
	{
		// printf("not exist in ambigous redirect\n"); //debug
		return (FALSE);
	}
	// printf("checking token value %s\n", (*token_list)->value); //debug
	current = *token_list;
	if (ft_strcmp((*token_list)->value, "") == 0)
	{
		
		return (FALSE);
	}
	// if (is_white_spaces_cmd((*token_list)->value))
	// {
	// 	printf("only white space \n");
	// 	return (FALSE);
		
	// }
	ori_value = current->ori_value;
	count_redir_file = 1;
	current = current->next;
	while (current && (current->type != PIPE && is_redirection(current) == FALSE))
	{
		if (ft_strcmp(current->ori_value, ori_value) == 0)
			count_redir_file++;
		current = current->next;
	}
	if (count_redir_file != 1)
		return (FALSE);
	return (TRUE);
}
