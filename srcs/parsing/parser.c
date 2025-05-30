/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:29:53 by trpham            #+#    #+#             */
/*   Updated: 2025/05/26 19:06:40 by trpham           ###   ########.fr       */
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
	(*new_cmd)->args = fill_args(temp_token_list);
	// print_array((*new_cmd)->args);
	if (!(*new_cmd)->args)
	{
		print_error("Failed to update new cmd args");
		return (FALSE);
	}
	// (*new_cmd)->cmd_name = ft_strdup(((*new_cmd)->args)[0]);
	// if (!(*new_cmd)->cmd_name)
	// 	return (FALSE);
	// H add
	if ((*new_cmd)->args[0])
		(*new_cmd)->cmd_name = ft_strdup((*new_cmd)->args[0]);
	else
		(*new_cmd)->cmd_name = NULL;
	if (parse_redirection(new_cmd, temp_token_list) == FALSE)
	{
		print_error("Parse redirection failed \n");
		return (FALSE);
	}
	return (TRUE);
}

// remove !*token_list condition,
//	cause that's when the token_list reaching the end, not an error
int	parse_redirection(t_cmd **new_cmd, t_token **token_list)
{
	t_redirect_type	redir_type;

	if (!new_cmd || !token_list)
	{
		print_error("Pointer to new cmdn and token list not exist");
		return (FALSE);
	}
	else if (!*new_cmd)
	{
		print_error("Invalid new_cmd in parse_redirection");
		return (FALSE);
	}
	while (*token_list && (*token_list)->type != PIPE)
	{
		if (is_redirection(*token_list) == TRUE)
		{
			// printf("Redirect type %s\n", (*token_list)->value);
			redir_type = token_to_redirect_type((*token_list)->type);
			if (redir_type == REDIR_INVALID)
			{
				print_error("invalid redirection token type");
				return (FALSE);
			}
			// printf("redir type %d\n", redir_type);
			(*token_list) = (*token_list)->next;
			// printf("after redirects %s\n", (*token_list)->value);
			if (!*token_list || (*token_list)->type != WORD
				|| !(*token_list)->value || !*(*token_list)->value)
			{
				print_error("Invalid or missing file after redirection");
				// get_error_msg(ERR_REDIR);
				return (FALSE);
			}
			if (add_redirects(&(*new_cmd)->redirects, redir_type,
					(*token_list)->value) == FALSE)
			{
				print_error("Failed to add redirects");
				return (FALSE);
			}
			(*token_list) = (*token_list)->next;
		}
		else
			(*token_list) = (*token_list)->next;
	}
	return (TRUE);
}

int	add_redirects(t_redirect **redir_list, t_redirect_type type, char *file)
{
	t_redirect	*new_redir;
	t_redirect	*current;

	new_redir = NULL;
	current = NULL;
	if (!file || !*file)
	{
		print_error("Empty file name in redirection");
		return (FALSE);
	}
	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
	{
		print_error("Failed to add redirects");
		// get_error_msg(ERR_MALLOC);
		return (FALSE);
	}
	new_redir->file = ft_strdup(file);
	if (!new_redir->file)
	{
		// free(new_redir);
		// get_error_msg(ERR_MALLOC);
		print_error("Failed to strdup file");
		return (FALSE);
	}
	new_redir->type = type;
	if (type == REDIR_IN)
		new_redir->fd = 0;
	else if (type == REDIR_HEREDOC)
		new_redir->fd = 1;
	new_redir->ori_path = NULL;
	new_redir->tmp_file = NULL;
	new_redir->next = NULL;
	// printf("add redirects workd\n");
	if (!*redir_list)
	{
		// printf("redir list is NULL\n");
		*redir_list = new_redir;
		// printf("redir list %s\n", (*redir_list)->file);
	}
	else
	{
		current = *redir_list;
		// printf("current %s\n", current->file);
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	// printf("ok here add redirects\n");
	return (TRUE);
}

t_redirect_type	token_to_redirect_type(t_token_type token_type)
{
	if (token_type == REDIR_IN_TOKEN)
		return (REDIR_IN);
	else if (token_type == REDIR_OUT_TOKEN)
		return (REDIR_OUT);
	else if (token_type == REDIR_APPEND_TOKEN)
		return (REDIR_APPEND);
	else if (token_type == REDIR_HEREDOC_TOKEN)
		return (REDIR_HEREDOC);
	return (REDIR_INVALID);
}

char	**fill_args(t_token **token_list)
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
		// get_error_msg(ERR_MALLOC);
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
				print_error("Fill argurment malloc error");
				free_array(args, count);
				return (NULL);
			}
			count++; // only increase if args[count]
		}
		else if (is_redirection(*token_list) == TRUE)
		{
			args[count] = NULL;
			return (args);
		}
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
		print_error("Malloc failed to create new_cmd");
		// get_error_msg(ERR_MALLOC);
		return (NULL);
	}
	new_cmd->args = NULL;
	new_cmd->cmd_name = NULL;
	new_cmd->redirects = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	free_cmd_list(t_cmd *head)
{
	t_cmd *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		// free(temp->args);
		free(temp->cmd_name);
		free(temp);
	}
	head = NULL;
}
