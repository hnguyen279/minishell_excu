/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:29:53 by trpham            #+#    #+#             */
/*   Updated: 2025/05/15 17:43:41 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_cmd	*parse_tokens_to_commands(t_token *tokenized_input_list)
{
	t_token *temp_token_list = NULL;
	t_cmd	*cmd_list = NULL;
	t_cmd	*new_cmd;
	t_cmd	*current;

	temp_token_list = tokenized_input_list;
	while (temp_token_list)
	{
		new_cmd = create_cmd();
		if (!new_cmd)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (update_command_node(&new_cmd, &temp_token_list) == FALSE)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (cmd_list == NULL)
			cmd_list = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		if (temp_token_list)
			temp_token_list = temp_token_list->next;
	}
	return (cmd_list);	
}


int	update_command_node(t_cmd **new_cmd, t_token **temp_token_list)
{
	(*new_cmd)->args = fill_args(temp_token_list);
	// print_array((*new_cmd)->args);
	if (!(*new_cmd)->args)
	{
		// printf("failed here \n");
		return (FALSE);
	}
	// printf("failed here \n");
	(*new_cmd)->cmd_name = ft_strdup(((*new_cmd)->args)[0]);
	if (!(*new_cmd)->cmd_name)
		return (FALSE);
	// printf("cmd name succeed \n");
	if (parse_redirection(new_cmd, temp_token_list) == FALSE)
		return (FALSE);	
	return (TRUE);
}

int	parse_redirection(t_cmd **new_cmd, t_token **token_list)
{
	while (*token_list && (*token_list)->type != PIPE)
	{
		// printf("Redirection ? %s\n", token_list->value);
		if ((*token_list)->type == IN || (*token_list)->type == HEREDOC)
		{
			// printf("Redirection IN or HEREDOC\n");
			(*token_list) = (*token_list)->next;			
			if ((*token_list) && (*token_list)->type == WORD)
				(*new_cmd)->infile = ft_strdup((*token_list)->value);
			if (!(*new_cmd)->infile)
			{
				get_error_msg(ERR_MALLOC);
				return (FALSE);
			}
		}
		else if ((*token_list)->type == OUT || (*token_list)->type == APPEND)
		{
			(*token_list) = (*token_list)->next;
			if ((*token_list) && (*token_list)->type == WORD)
				(*new_cmd)->outfile = ft_strdup((*token_list)->value);	
			if (!(*new_cmd)->outfile)
			{
				get_error_msg(ERR_MALLOC);
				return (FALSE);
			}
		}
		(*token_list) = (*token_list)->next;
	}
	return (TRUE);
}

char	**fill_args(t_token **token_list)
{
	int		count;
	char	**args;

	count = count_args(*token_list);
	// printf("count args %d\n", count);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		get_error_msg(ERR_MALLOC);
		return (NULL);
	}
	count = 0;
	while (*token_list && (*token_list)->type != PIPE)
	{
		if ((*token_list)->type == WORD)
		{
			args[count] = ft_strdup((*token_list)->value);
			if (!args[count])
			{
				get_error_msg(ERR_MALLOC);
				return (NULL);
			}
		}
		else if (is_redirection(*token_list) == TRUE)
			return (args);
		count++;
		*token_list = (*token_list)->next;
	}
	args[count] = NULL;
	return (args);
}

int	count_args(t_token *tokenized_input_list)
{
	t_token	*temp;
	int		count;
	
	temp = tokenized_input_list;
	count = 0;
	while (temp && temp->type != PIPE)
	{
		if (temp->type == WORD)
			count++;
		temp = temp->next;
	}
	return (count);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
	{
		get_error_msg(ERR_MALLOC);
		return (NULL);
	}
	new_cmd->args = NULL;
	new_cmd->cmd_name = NULL;
	new_cmd->infile = NULL;
	new_cmd->outfile = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}



void free_cmd_list(t_cmd *head)
{
	t_cmd	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
	
}