/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:04 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 16:06:06 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	execute_ast(t_ast *node, t_shell *mshell)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		mshell->has_pipe = 1;
		return (execute_pipe(node, mshell));
	}
	else if (node->type == NODE_CMD)
	{
		mshell->has_pipe = 0;
		return (execute_command(node, mshell));
	}
	ft_printf_fd(2, "minishell: unsupported node type\n");
	return (mshell->exit_code = 1);
}
