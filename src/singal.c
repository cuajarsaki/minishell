/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:57:16 by pchung            #+#    #+#             */
/*   Updated: 2025/03/02 00:17:33 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

volatile sig_atomic_t g_signal_received = 0;

static int event_hook_readline(void);

void init_readline_for_signal(void)
{
    rl_outstream = stderr;
    rl_done = 0;
    rl_catch_signals = 0;
    rl_event_hook = event_hook_readline;
}

static int event_hook_readline(void)
{
    if (g_signal_received == 1)
    {
        rl_replace_line("", 0);
        rl_done = 1;
    }
    return (0);
}

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = 1;
    write(STDOUT_FILENO, "^C", 3);
}

void init_signal(void (*handler_for_sigint)(int),
                 void (*handler_for_sigquit)(int))
{
    set_signal(SIGINT, handler_for_sigint, 0);
    set_signal(SIGQUIT, handler_for_sigquit, 0);
}

void set_signal(int signum, void (*handler)(int), int flags)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags;
    sa.sa_handler = handler;
    sigaction(signum, &sa, NULL);
}

void setup_signals(void)
{
    set_signal(SIGINT, handle_sigint, 0);
    set_signal(SIGQUIT, SIG_IGN, 0); 
    set_signal(SIGTSTP, SIG_IGN, 0); 
}