
#include "../../includes/shell.h"

volatile sig_atomic_t g_signum = 0;

static void sigint_interactive(int sig)
{
	(void)sig;
    g_signum = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
    rl_replace_line("", 0);
	rl_redisplay();
}

static void sigint_heredoc(int sig)
{
	(void)sig;
    g_signum = SIGINT;
	rl_done = 1;
    int null_fd = open("/dev/null", O_RDONLY);
    if (null_fd != -1)
    {
        dup2(null_fd, STDIN_FILENO);
        close(null_fd);
    }
    write(STDOUT_FILENO, "\n", 1);
}

int setup_signal_handlers(t_shell *mshell, void (*sigint_handler)(int), void (*sigquit_handler)(int))
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_int.sa_mask) == -1)
        display_error_errno(mshell, "sigemptyset", 1);
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        display_error_errno(mshell, "sigaction(SIGINT)", 1);
    sa_quit.sa_handler = sigquit_handler;
    sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigemptyset(&sa_quit.sa_mask) == -1)
        display_error_errno(mshell, "sigemptyset", 1);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        display_error_errno(mshell, "sigaction(SIGQUIT)", 1);
    return (0);
}


void setup_signals(t_shell *mshell, int mode)
{
    if (mode == MODE_INTERACTIVE)
        setup_signal_handlers(mshell, sigint_interactive, SIG_IGN);
    else if (mode == MODE_HEREDOC)
        setup_signal_handlers(mshell, sigint_heredoc, SIG_IGN);
    else // CHILD_STATE
        setup_signal_handlers(mshell, SIG_DFL, SIG_DFL);
}

// void sig_exit_code(t_shell *mshell)
// {
//     if (g_signum == 128 + SIGINT)
//         mshell->exit_code = 130;
//     else if (g_signum == 128 + SIGQUIT)
//         mshell->exit_code = 131;
//     else if (g_signum != 0)
//         mshell->exit_code = g_signum + 128;
//     g_signum = 0;
// }

void sig_exit_code(t_shell *mshell)
{
    if (g_signum == SIGINT)
        mshell->exit_code = 128 + SIGINT;  // = 130
    else if (g_signum == SIGQUIT)
        mshell->exit_code = 128 + SIGQUIT; // = 131
    else if (g_signum != 0)
        mshell->exit_code = 128 + g_signum;
    g_signum = 0;
}
