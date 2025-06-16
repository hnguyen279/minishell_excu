/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:47:20 by trpham            #+#    #+#             */
/*   Updated: 2025/06/16 22:23:05 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
Using a linked list to maintain history list
*/
int	store_history(char *line, t_token **history_head)
{
	t_token	*new_hist;
	t_token	*temp;

	add_history(line);
	new_hist = create_token(line, line, WORD);
	if (!new_hist)
	{
		free(new_hist);
		print_error("Failed to create working history");
		return (FALSE);
	}
	if (!*history_head)
		*history_head = new_hist;
	else
	{
		temp = *history_head;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new_hist;
	}
	return (TRUE);
}

void	print_working_history(t_token *history_head)
{
	int		i;
	t_token	*temp;

	i = 1;
	temp = history_head;
	while (temp)
	{
		if (temp)
		{
			if (i < 10)
				printf("   %d  %s\n", i, temp->value);
			else if (i < 100)
				printf("  %d  %s\n", i, temp->value);
			else if (i < 1000)
				printf(" %d  %s\n", i, temp->value);
			else
				printf("%d  %s\n", i, temp->value);
		}
		temp = temp->next;
		i++;
	}
}

void	clear_working_history(t_token **hist_head)
{
	t_token	*temp;

	while (*hist_head)
	{
		temp = *hist_head;
		*hist_head = (*hist_head)->next;
		free_string(temp->value);
		free_string(temp->ori_value);
		free(temp);
	}
	*hist_head = NULL;
	// printf("clean wokring history\n"); //debug
	// rl_clear_history();
}
