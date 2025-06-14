/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:19:11 by trpham            #+#    #+#             */
/*   Updated: 2025/06/14 07:44:44 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// void	handle_line(char *line, t_shell *mshell)
// {
// 	t_token	*token_list;
// 	t_cmd	*cmd_list;
// 	t_ast	*tree;

// 	token_list = NULL;
// 	if (init_and_validate_input(line, mshell, &token_list) == FALSE)
// 	{
// 		return ;
// 	}
// 	cmd_list = NULL;
// 	tree = NULL;
// 	process_valid_line(mshell, &token_list, &cmd_list, &tree);
// 	free_token_list(token_list);
// 	token_list = NULL;
// 	free_cmd_list(cmd_list);
// 	free_ast(tree, mshell);
// 	mshell->ast = NULL;
// 	tree = NULL;
// }

// int	init_and_validate_input(char *line, t_shell *mshell, t_token **token_list)
// {
// 	mshell->heredoc_index = 0;
// 	// if (ft_strcmp(line, " ") == 0)
// 	// 	return (FALSE); // or true?
// 	if (validate_quote(line) == FALSE)
// 	{
// 		mshell->exit_code = 2;
// 		return (FALSE);
// 	}
// 	if (tokenization_expansion_validation(line, mshell, token_list) == FALSE
// 		|| skip_expanded_empty_var(token_list) == FALSE)
// 	{
// 		free_token_list(*token_list);
// 		*token_list = NULL;
// 		return (FALSE);
// 	}
// 	return (TRUE);
// }

// int	tokenization_expansion_validation(char *line, t_shell *mshell,
// 		t_token **token_list)
// {
// 	*token_list = convert_user_input_to_token(line, mshell);
// 	if (!*token_list)
// 	{
// 		mshell->exit_code = 1;
// 		return (FALSE);
// 	}
// 	if (validate_token(*token_list) == FALSE)
// 	{
// 		mshell->exit_code = 2;
// 		return (FALSE);
// 	}
// 	return (TRUE);
// }

// void	process_valid_line(t_shell *mshell, t_token **token_list,
// 		t_cmd **cmd_list, t_ast **tree)
// {
// 	print_linked_list(*token_list); //debug
// 	*cmd_list = parse_tokens_to_commands(*token_list);
// 	// *cmd_list = NULL;
// 	if (!*cmd_list)
// 	{
// 		mshell->exit_code = 0; // yes return 0 -> true
// 		return ;
// 	}
// 	// print_cmd_list(*cmd_list); //debug
// 	*tree = convert_cmd_to_ast(*cmd_list);
// 	if (!*tree)
// 	{
// 		mshell->exit_code = 1; // for fail by our code
// 		return ;
// 	}
// 	run_ast_pipeline(mshell, *tree);
// }

// void	run_ast_pipeline(t_shell *mshell, t_ast *tree)
// {
// 	if (process_heredocs(mshell, tree))
// 	{
// 		cleanup_heredoc_tempfiles(tree);
// 		mshell->exit_code = 130;
// 		return ;
// 	}
// 	execute_ast(tree, mshell);
// 	cleanup_heredoc_tempfiles(tree);
// 	if (tree->cmd && tree->cmd[0] && !mshell->has_pipe)
// 		// tree->type == NODE_CMD &&
// 		env_set_last_argument(mshell, tree->cmd);
// }





int	init_and_validate_input(char *line, t_shell *mshell, t_token **token_list)
{
	mshell->heredoc_index = 0;
	// if (ft_strcmp(line, " ") == 0)
	// 	return (FALSE); // or true?
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
	mshell->token_list = *token_list;

	if (validate_token(*token_list) == FALSE)
	{
		mshell->exit_code = 2;
		return (FALSE);
	}
	return (TRUE);
}



void	handle_line(char *line, t_shell *mshell)
{
	
	//mshell->token_list = NULL;
	if (init_and_validate_input(line, mshell, &mshell->token_list) == FALSE)
	{
		return ;
	}
	//mshell->cmd_list = NULL;
	//mshell->tree = NULL;
	process_valid_line(mshell);
	//free_token_list(mshell->token_list);
	//mshell->token_list = NULL;
	//free_cmd_list(mshell->cmd_list);
	//free_ast(mshell->tree, mshell);
}


void	process_valid_line(t_shell *mshell)
{
	//print_linked_list(*token_list); //debug
	mshell->cmd_list = parse_tokens_to_commands(mshell->token_list);
	// *cmd_list = NULL;
	if (!mshell->cmd_list)
	{
		mshell->exit_code = 0; // yes return 0 -> true
		return ;
	}
	// print_cmd_list(*cmd_list); //debug
	mshell->tree = convert_cmd_to_ast(mshell->cmd_list);
	if (!mshell->tree)
	{
		mshell->exit_code = 1; // for fail by our code
		return ;
	}
	run_ast_pipeline(mshell);
}

void run_ast_pipeline(t_shell *mshell)
{
	t_ast *tree;

	tree = mshell->tree;
	if (!tree)
		return ;
	if (process_heredocs(mshell, tree))
	{
		cleanup_heredoc_tempfiles(tree);
		mshell->exit_code = 130;
		return;
	}
	execute_ast(tree, mshell); //start execution use current node = root tree
	cleanup_heredoc_tempfiles(tree);

	if (tree->cmd && tree->cmd[0] && !mshell->has_pipe)
		env_set_last_argument(mshell, tree->cmd);
}
