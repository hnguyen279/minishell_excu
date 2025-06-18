/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:07:55 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/08 18:07:55 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	sig_exit_code(t_shell *mshell)
{
	if (g_signum == SIGINT || g_signum == SIGQUIT)
		mshell->exit_code = 128 + g_signum;
	g_signum = 0;
}

int	child_default_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR)
		return (1);
	return (0);
}
