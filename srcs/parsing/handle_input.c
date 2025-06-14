/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:19:11 by trpham            #+#    #+#             */
/*   Updated: 2025/06/14 13:50:02 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	process_valid_line(t_shell *mshell)
{
	// print_linked_list(mshell->token_list); //debug
	mshell->cmd_list = parse_tokens_to_commands(mshell->token_list);
	// *cmd_list = NULL;
	if (!mshell->cmd_list)
	{
		mshell->exit_code = 0; // yes return 0 -> true
		return ;
	}
	// print_cmd_list(mshell->cmd_list); //debug
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
