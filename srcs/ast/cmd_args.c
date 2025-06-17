/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 16:58:13 by trpham            #+#    #+#             */
/*   Updated: 2025/06/17 12:58:55 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**fill_args(t_cmd **new_cmd, t_token **token_list)
{
	int		count;
	char	**args;

	count = count_args(*token_list);
	args = allocate_arg_array(count);
	if (!args)
		return (NULL);
	if (count == 0)
		return (args);
	if (fill_args_loop(token_list, args, new_cmd) == FALSE)
	{
		free_array_null(&args);
		return (NULL);
	}
	return (args);
}

char	**allocate_arg_array(int count)
{
	char	**args;

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
	args = ft_calloc((count + 1), sizeof(char *));
	if (!args)
	{
		print_error("Malloc failed");
		return (NULL);
	}
	return (args);
}

int	fill_args_loop(t_token **token_list, char **args, t_cmd **new_cmd)
{
	int	count;

	count = 0;
	while (*token_list && (*token_list)->type != PIPE)
	{
		if ((*token_list)->type == WORD)
		{
			args[count] = ft_strdup((*token_list)->value);
			if (!args[count])
			{
				print_error("Strdup malloc error");
				free_array_null(&args);
				return (FALSE);
			}
			count++;
			*token_list = (*token_list)->next;
		}
		else if (is_redirection(*token_list) == TRUE)
		{
			if (parse_redirection(new_cmd, token_list) == FALSE)
				return (FALSE);
		}
	}
	args[count] = NULL;
	return (TRUE);
}
