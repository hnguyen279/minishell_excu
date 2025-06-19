/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:04 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/19 05:20:10 by thi-huon         ###   ########.fr       */
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
	else
		mshell->exit_code = 1;
}
