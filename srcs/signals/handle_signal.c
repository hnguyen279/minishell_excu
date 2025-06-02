
#include "../../includes/shell.h"

volatile sig_atomic_t g_signum = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    g_signum = SIGINT;
}
static int reset_readline_interactive(void)
{
    if (g_signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_done = 1;
    }
    return 0;
}

// static int reset_readline_heredoc(void)
// {
//     if (g_signum == SIGINT)
//     {
//         rl_replace_line("", 0);
//         rl_on_new_line();
//         rl_redisplay();
//         rl_done = 1;
//     }
//     return 0;
// }
int reset_readline_heredoc(void)
{
    if (g_signum == SIGINT)
    {
        //debug
        write(2, "HEREDOC SIGINT CAUGHT\n", 23);
        write(STDOUT_FILENO, "\n", 1); // Print newline for clean prompt
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_done = 1; // Exit readline
    }
    return 0;
}

static int set_signal(t_shell *mshell, void (*sigint_handler)(int), void (*sigquit_handler)(int))
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

// Setup signal mode
void setup_signals(t_shell *mshell, int mode)
{
    g_signum = 0;
    if (mode == MODE_INTERACTIVE)
    {
        set_signal(mshell, handle_sigint, SIG_IGN);
        rl_event_hook = reset_readline_interactive;

    }
    else if (mode == MODE_HEREDOC)
    {
        set_signal(mshell, handle_sigint, SIG_IGN);
        rl_event_hook = reset_readline_heredoc;
    }
    else
    {
        set_signal(mshell, SIG_DFL, SIG_DFL);
        rl_event_hook = NULL;
    }
}

// Convert signal → exit code
void sig_exit_code(t_shell *mshell)
{
    if (g_signum == SIGINT || g_signum == SIGQUIT)
        mshell->exit_code = 128 + g_signum;
    g_signum = 0; // Always clear signal
}


