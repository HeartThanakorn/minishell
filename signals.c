#include "minishell.h"

// Global variable for signal handling (as required by the subject)
int g_signal = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    printf("\n");
    // For now, just print a new prompt indicator
    // In the full shell, this would trigger a new prompt
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Do nothing for ctrl-\ in interactive mode
}

void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_handler = handle_sigquit;
    sa_quit.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
