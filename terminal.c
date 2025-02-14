/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:21:32 by pchung            #+#    #+#             */
/*   Updated: 2025/02/14 17:34:29 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


void reset_terminal_settings(const struct termios *old_termios)
{
    if (tcsetattr(STDIN_FILENO, TCSANOW, old_termios) < 0)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void setup_terminal(struct termios *old_termios)
{
    if (!isatty(STDIN_FILENO))
    {
        perror("Standard input is not a terminal");
        exit(EXIT_FAILURE);
    }

    if (tcgetattr(STDIN_FILENO, old_termios) < 0)
    {
        perror("Failed to get terminal attributes");
        exit(EXIT_FAILURE);
    }

    struct termios new_termios = *old_termios;

    // Disable canonical mode, echo, signals, and other features
    new_termios.c_lflag &= ~(ICANON | ECHO | IEXTEN);
    new_termios.c_iflag &= ~(IXON);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) < 0)
    {
        perror("Failed to set terminal attributes");
        exit(EXIT_FAILURE);
    }

    if (tgetent(NULL, getenv("TERM")) < 0)
    {
        perror("Failed to initialize termcap");
        exit(EXIT_FAILURE);
    }
}

void term_clear_screen()
{
    char *clear_cmd = tgetstr("cl", NULL);
    if (clear_cmd)
        tputs(clear_cmd, 1, putchar);
    else
        write(STDERR_FILENO, "Clear screen capability not supported.\n", 40);
}
