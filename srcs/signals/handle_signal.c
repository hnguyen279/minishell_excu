/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trpham <trpham@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:03:24 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/17 19:21:37 by trpham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

volatile sig_atomic_t	g_signum = 0;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_signum = SIGINT;
}

static int	reset_readline_interactive(void)
{
	if (g_signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		rl_done = 1;
	}
	return (0);
}

static int	reset_readline_heredoc(void)
{
	if (g_signum == SIGINT)
	{
		rl_done = 1;
	}
	return (0);
}

static int	set_signal(t_shell *mshell, void (*sigint_handler)(int),
		void (*sigquit_handler)(int))
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sa_int.sa_flags = SA_RESTART;
	if (sigemptyset(&sa_int.sa_mask) == -1)
		error_msg(mshell, "sigemptyset", 1);
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		error_msg(mshell, "sigaction(SIGINT)", 1);
	sa_quit.sa_handler = sigquit_handler;
	sa_quit.sa_flags = SA_RESTART;
	if (sigemptyset(&sa_quit.sa_mask) == -1)
		error_msg(mshell, "sigemptyset", 1);
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		error_msg(mshell, "sigaction(SIGQUIT)", 1);
	return (0);
}

void	setup_signals(t_shell *mshell, int mode)
{
	g_signum = 0;
	if (mode == MODE_INTERACTIVE)
	{
		set_signal(mshell, handle_sigint, SIG_IGN);
		if (isatty(STDIN_FILENO))
			rl_event_hook = reset_readline_interactive;
	}
	else if (mode == MODE_HEREDOC)
	{
		set_signal(mshell, handle_sigint, SIG_IGN);
		if (isatty(STDIN_FILENO))
			rl_event_hook = reset_readline_heredoc;
	}
	else
	{
		set_signal(mshell, SIG_DFL, SIG_DFL);
		rl_event_hook = NULL;
	}
}
