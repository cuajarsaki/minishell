/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:57:16 by pchung            #+#    #+#             */
/*   Updated: 2025/03/04 17:06:53 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

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

