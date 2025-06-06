/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:29:53 by trpham            #+#    #+#             */
/*   Updated: 2025/06/06 15:36:06 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_cmd	*parse_tokens_to_commands(t_token *tokenized_list)
{
	t_token	*temp_token_list;
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	t_cmd	*current;

	temp_token_list = NULL;
	cmd_list = NULL;
	new_cmd = NULL;
	current = NULL;
	temp_token_list = tokenized_list;
	while (temp_token_list)
	{
		new_cmd = create_cmd();
		if (!new_cmd)
		{
			print_error("Failed to create cmd");
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (update_command_node(&new_cmd, &temp_token_list) == FALSE)
		{
			print_error("Failed to update command node");
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (cmd_list == NULL)
			cmd_list = new_cmd;
		else
		{
			current = cmd_list;
			while (current->next)
				current = current->next;
			current->next = new_cmd;
		}
		current = NULL;
		if (temp_token_list)
			temp_token_list = temp_token_list->next;
	}
	return (cmd_list);
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
		print_error("Failed creating new_cmd");
		return (FALSE);
	}
	else if (!*temp_token_list)
	{
		print_error("Not exist temp token list");
		return (FALSE);
	}
	(*new_cmd)->args = fill_args(new_cmd, temp_token_list);
	// print_array((*new_cmd)->args);
	if (!(*new_cmd)->args)
	{
		print_error("Failed to update new cmd args");
		return (FALSE);
	}
	if ((*new_cmd)->args[0])
		(*new_cmd)->cmd_name = ft_strdup((*new_cmd)->args[0]);
	else
		(*new_cmd)->cmd_name = NULL;
	return (TRUE);
}

char	**fill_args(t_cmd **new_cmd, t_token **token_list)
{
	int		count;
	char	**args;

	count = count_args(*token_list);
	// H add
	if (count == 0)
	{
		args = malloc(sizeof(char *));
		if (!args)
		{
			print_error("Malloc failed");
			return (NULL);
		}
		args[0] = NULL;
		return (args);
	}
	// printf("count args %d\n", count);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		print_error("Malloc failed");
		return (NULL);
	}
	count = 0;
	while (*token_list && (*token_list)->type != PIPE)
	{
		// printf("Working on this token %s\n", (*token_list)->value);

		if ((*token_list)->type == WORD)
		{
			args[count] = ft_strdup((*token_list)->value);
			if (!args[count])
			{
				print_error("Fill argurment malloc error");
				free_array(args, count);
				return (NULL);
			}
			count++; // only increase if args[count] is WORD
			*token_list = (*token_list)->next;
		}
		else if (is_redirection(*token_list) == TRUE)
		{
			if (parse_redirection(new_cmd, token_list) == FALSE)
			{
				print_error("Parse redirection failed \n");
				return (NULL);
			}
		}
	}
	args[count] = NULL;
	return (args);
}
