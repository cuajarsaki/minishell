#include "../shell.h"
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

static struct termios old_termios, new_termios;

void reset_terminal_settings()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

void setup_terminal()
{

	if (!isatty(STDIN_FILENO))
	{
		perror("Standard input is not a terminal");
		exit(EXIT_FAILURE);
	}

    if (tcgetattr(STDIN_FILENO, &old_termios) < 0)
    {
        perror("Failed to get terminal attributes");
        exit(EXIT_FAILURE);
    }

    new_termios = old_termios;

    // Disable canonical mode, echo, signals, and other features
    new_termios.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    new_termios.c_iflag &= ~(IXON);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) < 0)
    {
        perror("Failed to set terminal attributes");
        exit(EXIT_FAILURE);
    }

    atexit(reset_terminal_settings);

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

void move_cursor(int row, int col)
{
    char *cursor_cmd = tgetstr("cm", NULL);
    if (cursor_cmd)
        tputs(tgoto(cursor_cmd, col, row), 1, putchar);
    else
		fprintf(stderr, "Cursor movement capability not supported.\n");

}

void reset_cursor()
{
    move_cursor(0, 0);
}

void handle_backspace(char *buf, size_t *len)
{
    if (*len > 0)
    {
        buf[--(*len)] = '\0';
        write(STDOUT_FILENO, "\b \b", 3); // Erase the last character visually
    }
}

void	reset_cmd_line(char *buf, size_t *len)
{
            write(STDOUT_FILENO, "^C\n", 3);
            buf[0] = '\0';
            *len = 0; 
}

void	exit_program()
{
	exit(0);
}

void handle_input(char *buf, size_t *len, size_t max_len) {
    char c;

    while (read(STDIN_FILENO, &c, 1) > 0) {
        if (c == '\033') { // Escape character
            char seq[3] = {0};
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                if (seq[1] == 'A') { // Up arrow
                    const char *prev_command = get_history(-1);
                    if (prev_command) {
                        // Clear the buffer and load the previous command
                        ft_memset(buf, 0, *len);
                        *len = ft_strlen(prev_command);
                        ft_strncpy(buf, prev_command, *len);

                        // Clear the line and display the command
                        write(STDOUT_FILENO, "\r\033[K", 4); // Clear line
                        write(STDOUT_FILENO, "minishell ❤  ", 14); // Print prompt with one extra space
                        write(STDOUT_FILENO, buf, *len);
                    }
                } else if (seq[1] == 'B') { // Down arrow
                    const char *next_command = get_history(1);
                    if (next_command) {
                        // Clear the buffer and load the next command
                        ft_memset(buf, 0, *len);
                        *len = ft_strlen(next_command);
                        ft_strncpy(buf, next_command, *len);

                        // Clear the line and display the command
                        write(STDOUT_FILENO, "\r\033[K", 4); // Clear line
                        write(STDOUT_FILENO, "minishell ❤  ", 14); // Print prompt with one extra space
                        write(STDOUT_FILENO, buf, *len);
                    }
                }
            }

            continue;
        } else if (c == '\n') { // Enter key
            buf[*len] = '\0';
            write(STDOUT_FILENO, "\n", 1);
            add_to_history(buf); // Save the command to history
            return;
        } else if (c == 127) { // Backspace key
            if (*len > 0) {
                buf[--(*len)] = '\0'; // Remove the last character from the buffer
                write(STDOUT_FILENO, "\b \b", 3); // Move cursor back, clear the char, and move back again
            }
        } else if (c == 3) { // Ctrl-C
            write(STDOUT_FILENO, "^C\n", 3); // Visual feedback for Ctrl-C
            buf[0] = '\0'; // Clear the buffer
            *len = 0; // Reset length
            return; // Exit the current input
        } else if (c == 4) { // Ctrl-D
            if (*len == 0) { // If the buffer is empty, exit the program
                write(STDOUT_FILENO, "exit\n", 5); // Optional exit message
                exit(0);
            }
        } else if (*len < max_len - 1) {
            buf[(*len)++] = c;
            write(STDOUT_FILENO, &c, 1); // Echo the character
        }
    }
}


