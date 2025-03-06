/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:57:16 by pchung            #+#    #+#             */
/*   Updated: 2025/03/06 21:46:55 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

volatile sig_atomic_t	g_signal_received = 0;

static int	event_hook_readline(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

void	init_readline_for_signal(void)
{
	rl_outstream = stderr;
	rl_done = 0;
	rl_catch_signals = 0;
	rl_event_hook = event_hook_readline;
}
