/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:04 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/17 20:00:30 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	execute_ast(t_ast *node, t_shell *mshell)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		mshell->has_pipe = 1;
		execute_pipe(node, mshell);
	}
	else if (node->type == NODE_CMD)
	{
		execute_command(node, mshell);
	}
	else if (node->left)
		execute_ast(node->left, mshell);
	else if (node->right)
		execute_ast(node->right, mshell);
	while (wait(NULL) > 0)
		;
	mshell->exit_code = 1;
}
