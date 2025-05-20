
#include "../../includes/shell.h"

volatile sig_atomic_t g_signum = 0;

void	set_sigint_flag(int sig)
{
	if (sig == SIGINT)
        g_signum = sig;
}

int	reset_shell_prompt(void)
{
	if (g_signum == SIGINT)
	{
		g_signum = 128 + SIGINT;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
        rl_on_new_line();
		rl_redisplay();
	}
	return (0);
}

int	reset_heredoc_prompt(void)
{
	if (g_signum == SIGINT)
	{
		g_signum = 128 + SIGINT;
		write(STDOUT_FILENO, "^C\n", 3);
		rl_done = 1;
	}
	return (0);
}

int signal_error(t_shell *mshell, const char *message)
{
    ft_printf_fd(2, "%s: %s\n", message, strerror(errno));
    mshell->exit_code = 1;
    return (-1);
}


int setup_signal_handlers(t_shell *mshell, void (*sigint_handler)(int), void (*sigquit_handler)(int))
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_int.sa_mask) == -1)
        signal_error(mshell, "minishell: sigemptyset");
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        signal_error(mshell, "minishell: sigaction(SIGINT)");
    sa_quit.sa_handler = sigquit_handler;
    sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_quit.sa_mask) == -1)
        signal_error(mshell, "minishell: sigemptyset");
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        signal_error(mshell, "minishell: sigaction(SIGQUIT)");
    return (0);
}



static void heredoc_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_done = 1;
    g_signum = 128 + SIGINT;
}

void setup_signals(t_shell *mshell, int mode)
{
    if (mode == MODE_INTERACTIVE)
        setup_signal_handlers(mshell, set_sigint_flag, SIG_IGN);
    else if (mode == MODE_HEREDOC)
        setup_signal_handlers(mshell, heredoc_sigint, SIG_IGN);
    else // CHILD_STATE
        setup_signal_handlers(mshell, SIG_DFL, SIG_DFL);
}

void sig_exit_code(t_shell *mshell)
{
    if (g_signum == 128 + SIGINT)
        mshell->exit_code = 130;
    else if (g_signum == 128 + SIGQUIT)
        mshell->exit_code = 131;
    else if (g_signum != 0)
        mshell->exit_code = g_signum + 128;
    g_signum = 0;
}