/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:37:07 by trpham            #+#    #+#             */
/*   Updated: 2025/05/22 12:56:20 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	free_string(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void free_array(char **arr, int i)
{
	int	j;

	j = 0;

	if (!arr)
        return;
	while (j < i)
	{
		free_string(arr[j]);
		j++;
	}
	free(arr);
}

int	array_size(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		i++;
	}
	return (i);
}

void	print_linked_list(t_token *head)
{
	t_token	*temp;

	temp = head;
	printf("PRINT TOKEN START\n");
	while (temp)
	{
		if (temp)
		{
			printf("Value of input tokens %s type %d\n", temp->value, temp->type);
		}
		temp = temp->next;
	}
}

int	calculate_linked_list_size(t_token *head)
{
	t_token	*temp;
	int		len;

	temp = head;
	len = 0;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}

void	print_error(char *msg)
{
	if (msg)
	{
		ft_putstr_fd("Error: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\n", 2);
	}
}

void	get_error_msg(t_error_type err)
{
	if (err == ERR_MALLOC)
		print_error("Memory allocation failed");
	else if (err == ERR_PIPE)
		print_error("Unexpected pipe");
	else if (err == ERR_QUOTE)
		print_error("Unclosed quote");
	else if (err == ERR_SYNTAX)
		print_error("Syntax error");
	else if (err == ERR_REDIR)
		print_error("Unexpected redirection");
	else
		print_error("Undefined error");
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return TRUE;
	return FALSE;
}

void	print_array(char **arr)
{
	while (*arr)
	{
		printf("%s, ", *arr);
		arr++;
	}
	printf("\n");
}


void	print_cmd_list(t_cmd *head)
{
	t_cmd	*temp;
	int		i = 0;

	temp = head;
	while (temp)
	{
		printf("args %d: ", ++i);
		print_array(temp->args);
		// printf(" } \n");
		printf("cmd_name: %s\n", temp->cmd_name);
		print_redirect_list(temp->redirects);
		// printf("infile: %s\n", temp->infile);
		// printf("outfile: %s\n", temp->outfile);
		printf("next: %p\n", temp->next);
		// printf("redirect type :%d\n", temp->redirect_type);
		temp = temp->next;
	}
}

void	print_redirect_list(t_redirect *redir_list)
{
	t_redirect	*temp;
	int			i = 0;

	temp = redir_list;
	while (temp)
	{
		printf("redirect : %d\n", ++i);
		printf("fd : %d\n", redir_list->fd);
		printf("file : %s\n", redir_list->file);
		printf("original path : %s\n", redir_list->ori_path);
		printf("temp file : %s\n", redir_list->tmp_file);
		printf("redirect type : %d\n", redir_list->type);
		temp = temp->next;
	}
	
}

void free_token_list(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}