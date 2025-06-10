/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 16:58:13 by trpham            #+#    #+#             */
/*   Updated: 2025/06/10 15:34:41 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**fill_args(t_cmd **new_cmd, t_token **token_list)
{
	int		count;
	char	**args;

	count = count_args(*token_list);
	// print_linked_list(*token_list);
	// printf("return count arg %d\n", count);
	args = allocate_arg_array(count);
	if (!args)
		return (NULL);
	if (count == 0)
		return (args);
	if (fill_args_loop(token_list, args, new_cmd) == FALSE)
		return (NULL);
	// print_array(args); //debug
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
	args = (char **)malloc(sizeof(char *) * (count + 1));
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
			// printf("args: %s\n", (*token_list)->value); //debug
			args[count] = ft_strdup((*token_list)->value);
			if (!args[count])
			{
				print_error("Strdup malloc error");
				free_array(args, count);
				return (FALSE);
			}
			count++;
			*token_list = (*token_list)->next;
		}
		else if (is_redirection(*token_list) == TRUE)
		{
			if (handle_redirect(new_cmd, token_list, args, count) == FALSE)
				return (FALSE);
		}
	}
	args[count] = NULL;
	return (TRUE);
}

int	handle_redirect(t_cmd **new_cmd, t_token **token_list, char **args,
		int count)
{
	if (parse_redirection(new_cmd, token_list) == FALSE)
	{
		// print_error("Parse redirection failed \n");
		free_array(args, count);
		return (FALSE);
	}
	return (TRUE);
}
