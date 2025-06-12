/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:58:16 by trpham            #+#    #+#             */
/*   Updated: 2025/06/12 18:28:58 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*read_user_input(t_shell *mshell)
{
	char	*line;
	
	g_signum = 0;
	line = readline("minishell$ ");
	if (g_signum) // check siganl after readline for Ctrl C in main shell
		sig_exit_code(mshell);
	return (line);
}

int	process_user_line(char *line, t_token **history_head, t_shell *mshell)
{
	if (line[0] == '\0') // Ctrl+C → empty str//need?
		return (TRUE);
	store_history(line, history_head);
	if (handle_special_command_line(line, history_head) == TRUE)
	{
		if (ft_strcmp(line, "exit") == 0)
			return (FALSE);
	}
	else
		handle_line(line, mshell);
	return (TRUE);
}

int	handle_special_command_line(char *line, t_token **history_head)
{
	if (ft_strcmp(line, "exit") == 0)
		return (TRUE);
	else if (ft_strcmp(line, "history") == 0)
	{
		print_working_history(*history_head);
		return (TRUE);
	}
	else if (ft_strcmp(line, "history -c") == 0)
	{
		clear_working_history(history_head);
		return (TRUE);
	}
	return (FALSE);
}
