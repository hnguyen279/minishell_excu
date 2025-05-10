
#include "minishell.h"

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

static int signal_error(const char *signal_name)
{
    perror(signal_name);
    return -1;
}


int setup_signal_handlers(void (*sigint_handler)(int), void (*sigquit_handler)(int))
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP; 
    if (sigemptyset(&sa_int.sa_mask) == -1)
        return signal_error("minishell: sigemptyset");
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        return signal_error("minishell: sigaction(SIGINT)");
    sa_quit.sa_handler = sigquit_handler;
    sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP; 
    if (sigemptyset(&sa_quit.sa_mask) == -1)
        return signal_error("minishell: sigemptyset");
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        return signal_error("minishell: sigaction(SIGQUIT)");
    return (0);
}

int setup_signal_handlers(void (*sigint_handler)(int), void (*sigquit_handler)(int))
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_int.sa_mask) == -1 || sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("minishell: sigaction(SIGINT)");
        return (-1);
    }
    sa_quit.sa_handler = sigquit_handler;
    sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_quit.sa_mask) == -1 || sigaction(SIGQUIT, &sa_quit, NULL) == -1)
    {
        perror("minishell: sigaction(SIGQUIT)");
        return (-1);
    }
    return (0);
}

void setup_signals(t_signal_state mode)
{
    if (mode == INTERACTIVE_STATE)
        setup_signal_handlers(set_sigint_flag, SIG_IGN);
    else if (mode == HEREDOC_STATE)
        setup_signal_handlers(heredoc_sigint, SIG_IGN);
    else // CHILD_STATE
        setup_signal_handlers(SIG_DFL, SIG_DFL);
}

void	sig_exit_code(t_shell *mshell)
{
	if (g_status == 128 + SIGINT)
		ms->exit_code = 130;
	else if (g_status != 0)
		ms->exit_code = g_status + 128;
	g_status = 0;
}
