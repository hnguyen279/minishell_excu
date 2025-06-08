/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:06:32 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 16:17:45 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	exe_redirection(t_redirect *redir, t_shell *mshell)
{
	t_redirect	*current;
	int			result;

	current = redir;
	if (!mshell)
	{
		ft_printf_fd(2, "minishell: internal error: null shell pointer\n");
		return (1);
	}
	while (current)
	{
		result = handle_single_redirection(mshell, current);
		if (result != 0)
		{
			mshell->exit_code = result;
			return (result);
		}
		current = current->next;
	}
	mshell->exit_code = 0;
	return (0);
}
