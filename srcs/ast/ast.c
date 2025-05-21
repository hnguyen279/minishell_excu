/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 12:38:32 by trpham            #+#    #+#             */
/*   Updated: 2025/05/21 16:25:36 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void free_ast(t_ast *node, t_shell *mshell)
{
    if (!node)
        return;
    if (node->type == NODE_PIPE)
    {
        free_ast(node->left, mshell);
        free_ast(node->right, mshell);
    }
    if (node->type == NODE_CMD && node->cmd)
        ft_free_null(&node->cmd);
    t_redirect *redir = node->redirects;
    while (redir)
    {
        t_redirect *next = redir->next;
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

t_ast *convert_cmd_to_ast(t_cmd *cmd_list)
{
    if (!cmd_list)
        return (NULL);

    if (!cmd_list->next) // Single command
    {
        t_ast *node = malloc(sizeof(t_ast));
        if (!node)
        {
            get_error_msg(ERR_MALLOC);
            return (NULL);
        }
        node->type = NODE_CMD;
        node->cmd = cmd_list->args; // Transfer ownership
        node->redirects = cmd_list->redirects; // Transfer ownership
        node->left = NULL;
        node->right = NULL;
        free(cmd_list); // Free t_cmd, but not args or redirects
        return (node);
    }

    // Pipeline
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
    {
        get_error_msg(ERR_MALLOC);
        return (NULL);
    }
    node->type = NODE_PIPE;
    node->cmd = NULL;
    node->redirects = NULL;

    // Left child: first command
    t_cmd *left_cmd = cmd_list;
    cmd_list = cmd_list->next;
    left_cmd->next = NULL;
    node->left = convert_cmd_to_ast(left_cmd);

    // Right child: remaining commands
    node->right = convert_cmd_to_ast(cmd_list);

    return (node);
}


///////
// H
// t_ast *convert_cmd_to_ast(t_cmd *cmd_list)
// {
//     t_ast *left_node = NULL;
//     t_ast *pipe_node = NULL;

//     if (!cmd_list)
//         return (NULL);
//     left_node = create_ast_node(NODE_CMD);
//     if (!left_node)
//         return (NULL);
//     left_node->cmd = env_dup(cmd_list->args);
//     if (!left_node->cmd)
//     {
//         free(left_node);
//         return (NULL);
//     }
//     left_node->redirects = cmd_list->redirects;
//     if (!cmd_list->next)
//         return (left_node);
    
//     pipe_node = create_ast_node(NODE_PIPE);
//     if (!pipe_node)
//     {
//         ft_free_null(&left_node->cmd);
//         free(left_node);
//         return (NULL);
//     }
//     pipe_node->left = left_node;
//     cmd_list = cmd_list->next;
//     pipe_node->right = convert_cmd_to_ast(cmd_list);
//     return (pipe_node);
// }
// t_ast	*create_ast_node(int type)
// {
// 	t_ast	*new_node;

// 	new_node = malloc(sizeof(t_ast));
// 	if (!new_node)
// 	{
// 		get_error_msg(ERR_MALLOC);
// 		return (NULL);
// 	}
// 	new_node->type = type;
// 	new_node->cmd = NULL;
// 	new_node->redirects = NULL;
// 	new_node->left = NULL;
// 	new_node->right = NULL;
// 	return (new_node);
// }
// void	print_tree(t_ast *tree)
// {
// 	t_ast	*temp;
// 	int		i = 0;

// 	temp = tree;
// 	while (temp)
// 	{
// 		printf("node %d: ", ++i);
// 		print_cmd_list(temp->cmd);
// 		printf("tree node type: %d\n", temp->type);
// 		// printf("redirect type: %d\n", temp->redirects);
// 		printf("left: %p\n", temp->left);
// 		printf("right: %p\n", temp->right);
// 		// temp = temp->next;
// 	}
// }