/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:19:11 by trpham            #+#    #+#             */
/*   Updated: 2025/06/09 14:18:41 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	handle_line(char *line, t_shell *mshell)
{
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_ast	*tree;

	token_list = NULL;
	if (init_and_validate_input(line, mshell, &token_list) == FALSE)
		return ;
	cmd_list = NULL;
	tree = NULL;
	process_valid_line(mshell, &token_list, &cmd_list, &tree);
	free_token_list(token_list);
	token_list = NULL;
	free_cmd_list(cmd_list);
	free_ast(tree, mshell);
	mshell->ast = NULL;
	tree = NULL;
}

int	init_and_validate_input(char *line, t_shell *mshell, t_token **token_list)
{
	mshell->heredoc_index = 0;
	if (validate_quote(line) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	if (tokenization_expansion_validation(line, mshell, token_list) == FALSE
		|| empty_variable_extension(mshell, token_list) == FALSE)
	{
		free_token_list(*token_list);
		*token_list = NULL;
		return (FALSE);
	}
	// print_linked_list(*token_list);
	return (TRUE);
}

int	tokenization_expansion_validation(char *line, t_shell *mshell,
		t_token **tokenized_input_list)
{
	*tokenized_input_list = convert_user_input_to_token(line, mshell);
	if (!*tokenized_input_list)
	{
		mshell->exit_code = 1;
		return (FALSE);
	}
	if (validate_token(*tokenized_input_list) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	return (TRUE);
}

int	empty_variable_extension(t_shell *mshell, t_token **tokenized_input_list)
{
	t_token	*to_free;

	if (*tokenized_input_list && ft_strcmp((*tokenized_input_list)->value,
			"") == 0)
	{
		if ((*tokenized_input_list)->next)
		{
			to_free = *tokenized_input_list;
			(*tokenized_input_list) = (*tokenized_input_list)->next;
			free_string(to_free->value);
			free(to_free);
		}
		else
		{
			mshell->exit_code = 0;
			return (FALSE);
		}
	}
	return (TRUE);
}

void	process_valid_line(t_shell *mshell, t_token **token_list,
		t_cmd **cmd_list, t_ast **tree)
{
	*cmd_list = parse_tokens_to_commands(*token_list);
	if (!*cmd_list)
	{
		print_error("Failed to parse token to cmd");
		mshell->exit_code = 2;
		return ;
	}
	*tree = convert_cmd_to_ast(*cmd_list);
	if (!*tree)
	{
		mshell->exit_code = 2;
		print_error("Failed to convert cmd to ast");
		return ;
	}
	run_ast_pipeline(mshell, *tree);
}
