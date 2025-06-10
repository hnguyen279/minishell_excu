/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:29:53 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 11:08:28 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_cmd	*parse_tokens_to_commands(t_token *tokenized_list)
{
	t_token	*temp_token_list;
	t_cmd	*cmd_list;

	temp_token_list = NULL;
	cmd_list = NULL;
	temp_token_list = tokenized_list;
	while (temp_token_list)
	{
		if (add_and_update_cmd_node(&temp_token_list, &cmd_list) == FALSE)
			return (NULL);
		if (temp_token_list)
			temp_token_list = temp_token_list->next;
	}
	// if (ft_strcmp(cmd_list->args[0], "") == 0)
	// printf("return here\n");
	// print_cmd_list(cmd_list);
	return (cmd_list);
}

int	add_and_update_cmd_node(t_token **temp_token_list, t_cmd **cmd_list)
{
	t_cmd	*new_cmd;
	t_cmd	*current;

	new_cmd = NULL;
	current = NULL;
	new_cmd = create_cmd();
	// printf("return here 2\n");
	if (!new_cmd || update_command_node(&new_cmd, temp_token_list) == FALSE)
	{
		free_cmd_list(*cmd_list);
		return (FALSE);
	}
	if (*cmd_list == NULL)
		*cmd_list = new_cmd;
	else
	{
		current = *cmd_list;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
	current = NULL;
	return (TRUE);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
	{
		print_error("Malloc failed to create new_cmd");
		return (NULL);
	}
	new_cmd->args = NULL;
	new_cmd->cmd_name = NULL;
	new_cmd->redirects = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

int	update_command_node(t_cmd **new_cmd, t_token **temp_token_list)
{
	if (!*new_cmd)
	{
		// print_error("Failed creating new_cmd");
		return (FALSE);
	}
	else if (!*temp_token_list)
	{
		print_error("Not exist temp token list");
		return (FALSE);
	}
	(*new_cmd)->args = fill_args(new_cmd, temp_token_list);
	if (!(*new_cmd)->args)
	{
		// print_error("Failed to update new cmd args");
		return (FALSE);
	}
	if ((*new_cmd)->args[0])
		(*new_cmd)->cmd_name = ft_strdup((*new_cmd)->args[0]);
	else
	{
		printf("update command node failed \n");
		(*new_cmd)->cmd_name = NULL;
	}
	return (TRUE);
}
