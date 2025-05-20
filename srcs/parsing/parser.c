/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:29:53 by trpham            #+#    #+#             */
/*   Updated: 2025/05/17 06:57:44 by trpham           ###   ########.fr       */
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


// H add
int add_redirects(t_redirect **redir_list, t_redirect_type type, char *file)
{
    if (!file || !*file)
    {
        ft_printf_fd(STDERR_FILENO, "minishell: empty file name in redirection\n");
        return (FALSE);
    }

    t_redirect *new_redir = malloc(sizeof(t_redirect));
    if (!new_redir)
    {
        get_error_msg(ERR_MALLOC);
        return (FALSE);
    }

    new_redir->file = ft_strdup(file);
    if (!new_redir->file)
    {
        free(new_redir);
        get_error_msg(ERR_MALLOC);
        return (FALSE);
    }

    new_redir->type = type;
    new_redir->fd = (type == REDIR_IN || type == REDIR_HEREDOC) ? 0 : 1;
    new_redir->ori_path = NULL;
    new_redir->tmp_file = NULL;
    new_redir->next = NULL;

    if (!*redir_list)
        *redir_list = new_redir;
    else
    {
        t_redirect *current = *redir_list;
        while (current->next)
            current = current->next;
        current->next = new_redir;
    }

    return (TRUE);
}

t_redirect_type token_to_redirect_type(t_token_type token_type)
{
	if (token_type == REDIR_IN_token)
        return REDIR_IN;
    else if (token_type == REDIR_OUT_token)
        return REDIR_OUT;
    else if (token_type == REDIR_APPEND_token)
        return REDIR_APPEND;
    else if (token_type == REDIR_HEREDOC_token)
        return REDIR_HEREDOC;
    
    return REDIR_INVALID; 
}

int parse_redirection(t_cmd **new_cmd, t_token **token_list)
{
    if (!new_cmd || !*new_cmd || !token_list || !*token_list)
    {
        ft_printf_fd(STDERR_FILENO, "minishell: invalid args in parse_redirection\n");
        return (FALSE);
    }

    while (*token_list && (*token_list)->type != PIPE)
    {
        if (is_redirection(*token_list))
        {
            t_redirect_type redir_type = token_to_redirect_type((*token_list)->type);
            if (redir_type == REDIR_INVALID)
            {
                ft_printf_fd(STDERR_FILENO, "minishell: invalid redirection token type\n");
                return (FALSE);
            }

            *token_list = (*token_list)->next;
            if (!*token_list || (*token_list)->type != WORD || !(*token_list)->value || !*(*token_list)->value)
            {
                ft_printf_fd(STDERR_FILENO, "minishell: invalid or missing file after redirection\n");
                get_error_msg(ERR_REDIR);
                return (FALSE);
            }

            if (!add_redirects(&(*new_cmd)->redirects, redir_type, (*token_list)->value))
                return (FALSE);

            *token_list = (*token_list)->next;
        }
        else
        {
            *token_list = (*token_list)->next;
        }
    }

    return (TRUE);
}

// H hide
// void	add_redirects(t_redirect **redir_list, t_token_type type, char *file)
// {
// 	t_redirect	*new_redir;
// 	t_redirect	*current;

// 	new_redir = malloc(sizeof(t_redirect));
// 	if (!new_redir)
// 	{
// 		get_error_msg(ERR_MALLOC);
// 		return ;
// 	}
// 	if (type == REDIR_IN)
// 		new_redir->fd = 0;
// 	else if (type == REDIR_HEREDOC)
// 		new_redir->fd = 1;
// 	new_redir->type = type;
// 	new_redir->file = ft_strdup(file);
// 	if (!new_redir->file)
// 	{
// 		get_error_msg(ERR_MALLOC);
// 		return ;
// 	}
// 	new_redir->ori_path = NULL;
// 	new_redir->tmp_file = NULL;
// 	new_redir->next = NULL;
// 	if (!*redir_list)
// 		*redir_list = new_redir;
// 	else
// 	{
// 		current = *redir_list;
// 		while (current)
// 		{
// 			current = current->next;
// 		}
// 		current->next = new_redir;
// 		// (*redir_list)->next = new_redir;
// 		// current = new_redir;
// 	}
// }

// int	parse_redirection(t_cmd **new_cmd, t_token **token_list)
// {
// 	t_token_type	redir_type;
	
// 	while (*token_list && (*token_list)->type != PIPE)
// 	{
// 		if ( is_redirection(*token_list) == TRUE)
// 		{
// 			redir_type = (*token_list)->type;
// 			(*token_list) = (*token_list)->next;
// 			if ((*token_list) && (*token_list)->type == WORD)
// 				add_redirects(&(*new_cmd)->redirects, redir_type,
// 					(*token_list)->value);
// 			else
// 				return (FALSE);
// 		}
// 		// printf("Redirection ? %s\n", token_list->value);
// 		// if ((*token_list)->type == REDIR_IN || (*token_list)->type == REDIR_HEREDOC)
// 		// {
// 		// 	// (*new_cmd)->redirect_type = (*token_list)->type;
// 		// 	// printf("Redirection IN or HEREDOC\n");
// 		// 	(*token_list) = (*token_list)->next;			
// 		// 	if ((*token_list) && (*token_list)->type == WORD)
// 		// 		(*new_cmd)->infile = ft_strdup((*token_list)->value);
// 		// 	if (!(*new_cmd)->infile)
// 		// 	{
// 		// 		get_error_msg(ERR_MALLOC);
// 		// 		return (FALSE);
// 		// 	}
// 		// }
// 		// else if ((*token_list)->type == REDIR_OUT || (*token_list)->type == REDIR_APPEND)
// 		// {
// 		// 	// (*new_cmd)->redirect_type = (*token_list)->type;
// 		// 	(*token_list) = (*token_list)->next;
// 		// 	if ((*token_list) && (*token_list)->type == WORD)
// 		// 		(*new_cmd)->outfile = ft_strdup((*token_list)->value);	
// 		// 	if (!(*new_cmd)->outfile)
// 		// 	{
// 		// 		get_error_msg(ERR_MALLOC);
// 		// 		return (FALSE);
// 		// 	}
// 		// }
// 		(*token_list) = (*token_list)->next;
// 	}
// 	return (TRUE);
// }

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
	// new_cmd->infile = NULL;
	// new_cmd->outfile = NULL;
	new_cmd->redirects = NULL;
	new_cmd->next = NULL;
	// new_cmd->redirect_type = 0;
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