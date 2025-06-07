/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 12:38:32 by trpham            #+#    #+#             */
/*   Updated: 2025/06/07 13:57:38 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_ast	*convert_cmd_to_ast(t_cmd *cmd_list)
{
	t_ast	*left_node;
	t_ast	*pipe_node;

	left_node = NULL;
	pipe_node = NULL;
	if (!cmd_list)
		return (NULL);
	left_node = create_ast_node(NODE_CMD);
	if (!left_node)
		return (NULL);
	left_node->cmd = cmd_list->args;
	left_node->redirects = cmd_list->redirects;
	if (!cmd_list->next)
		return (left_node);
	pipe_node = create_ast_node(NODE_PIPE);
	if (!pipe_node)
		return (NULL);
	pipe_node->left = left_node;
	cmd_list = cmd_list->next;
	pipe_node->right = convert_cmd_to_ast(cmd_list);
	return (pipe_node);
}

void	free_ast(t_ast *node, t_shell *mshell)
{
	t_redirect	*redir;
	t_redirect	*next;

	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		free_ast(node->left, mshell);
		free_ast(node->right, mshell);
	}
	if (node->type == NODE_CMD && node->cmd)
		ft_free_null(&node->cmd);
	redir = node->redirects;
	while (redir)
	{
		next = redir->next;
		free_string(redir->file);
		free_string(redir->tmp_file);
		free_string(redir->ori_path);
		free(redir);
		redir = next;
	}
	free(node);
}

t_ast	*create_ast_node(int type)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
	{
		print_error("Failed to create ast node");
		return (NULL);
	}
	new_node->type = type;
	new_node->cmd = NULL;
	new_node->redirects = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}
