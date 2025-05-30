/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 12:38:32 by trpham            #+#    #+#             */
/*   Updated: 2025/05/26 18:55:48 by trpham           ###   ########.fr       */
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
	// printf("cmd list is not null\n");
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
		if (redir->file)
			free(redir->file);
		if (redir->tmp_file)
			free(redir->tmp_file);
		if (redir->ori_path)
			free(redir->ori_path);
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
		// get_error_msg(ERR_MALLOC);
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

// t_ast *convert_cmd_to_ast(t_cmd *cmd_list)
// {
//     if (!cmd_list)
//         return (NULL);
//     printf("cmd list is not null\n");
//     if (!cmd_list->next) // Single command
//     {
//         t_ast *node = malloc(sizeof(t_ast));
//         if (!node)
//         {
//             get_error_msg(ERR_MALLOC);
//             return (NULL);
//         }
//         node->type = NODE_CMD;
//         node->cmd = cmd_list->args; // Transfer ownership
//         node->redirects = cmd_list->redirects; // Transfer ownership
//         node->left = NULL;
//         node->right = NULL;
//         free(cmd_list); // Free t_cmd, but not args or redirects
//         return (node);
//     }

//     // Pipeline
//     t_ast *node = malloc(sizeof(t_ast));
//     if (!node)
//     {
//         get_error_msg(ERR_MALLOC);
//         return (NULL);
//     }
//     node->type = NODE_PIPE;
//     node->cmd = NULL;
//     node->redirects = NULL;

//     // Left child: first command
//     t_cmd *left_cmd = cmd_list;
//     cmd_list = cmd_list->next;
//     left_cmd->next = NULL;
//     node->left = convert_cmd_to_ast(left_cmd);

//     // Right child: remaining commands
//     node->right = convert_cmd_to_ast(cmd_list);

//     return (node);
// }
