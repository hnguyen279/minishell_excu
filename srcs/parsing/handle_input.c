/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:19:11 by trpham            #+#    #+#             */
/*   Updated: 2025/06/09 19:15:27 by trpham           ###   ########.fr       */
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
		|| skip_expanded_empty_var(token_list) == FALSE)
	{
		free_token_list(*token_list);
		*token_list = NULL;
		return (FALSE);
	}
	return (TRUE);
}

int	tokenization_expansion_validation(char *line, t_shell *mshell,
		t_token **token_list)
{
	*token_list = convert_user_input_to_token(line, mshell);
	if (!*token_list)
	{
		mshell->exit_code = 1;
		return (FALSE);
	}
	if (validate_token(*token_list) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	return (TRUE);
}

void	process_valid_line(t_shell *mshell, t_token **token_list,
		t_cmd **cmd_list, t_ast **tree)
{
	*cmd_list = parse_tokens_to_commands(*token_list);
	if (!*cmd_list)
	{
		mshell->exit_code = 2;
		return ;
	}
	*tree = convert_cmd_to_ast(*cmd_list);
	if (!*tree)
	{
		mshell->exit_code = 2;
		return ;
	}
	run_ast_pipeline(mshell, *tree);
}
