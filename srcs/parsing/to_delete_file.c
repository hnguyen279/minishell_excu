/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_delete_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:28:34 by trpham            #+#    #+#             */
/*   Updated: 2025/06/08 20:27:24 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

void	print_linked_list(t_token *head)
{
	t_token	*temp;

	temp = head;
	printf("PRINT TOKEN START\n");
	while (temp)
	{
		if (temp)
			printf("Value of input tokens %s type %d \n", temp->value,
				temp->type);
		temp = temp->next;
	}
}

void	print_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		printf("%s, ", arr[i]);
		i++;
	}
	printf("\n");
}

void	print_cmd_list(t_cmd *head)
{
	t_cmd	*temp;
	int		i;

	i = 0;
	temp = head;
	while (temp)
	{
		printf("args %d: ", ++i);
		print_array(temp->args);
		printf("cmd_name: %s\n", temp->cmd_name);
		print_redirect_list(temp->redirects);
		printf("next: %p\n", temp->next);
		temp = temp->next;
	}
}

void	print_redirect_list(t_redirect *redir_list)
{
	t_redirect	*temp;
	int			i;

	i = 0;
	temp = redir_list;
	while (temp)
	{
		printf("redirect : %d\n", ++i);
		printf("fd : %d\n", redir_list->fd);
		printf("file : %s\n", redir_list->file);
		printf("original path : %s\n", redir_list->ori_file);
		printf("temp file : %s\n", redir_list->tmp_file);
		printf("redirect type : %d\n", redir_list->type);
		temp = temp->next;
	}
}
