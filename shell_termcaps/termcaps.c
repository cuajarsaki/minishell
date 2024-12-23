#include "../shell.h"
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct termios old_termios, new_termios;

// Reset terminal settings to original state
void reset_terminal_settings()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// Set up the terminal for custom input handling
void setup_terminal()
{
	// Save current terminal settings
	if (tcgetattr(STDIN_FILENO, &old_termios) < 0)
	{
		perror("Failed to get terminal attributes");
		exit(EXIT_FAILURE);
	}

	new_termios = old_termios;

	// Disable canonical mode and local echo
	new_termios.c_lflag &= ~(ICANON | ECHO);

	// Apply new settings
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) < 0)
	{
		perror("Failed to set terminal attributes");
		exit(EXIT_FAILURE);
	}

	// Ensure terminal is reset on program exit
	atexit(reset_terminal_settings);

	// Initialize termcap database
	if (tgetent(NULL, getenv("TERM")) < 0)
	{
		perror("Failed to initialize termcap");
		exit(EXIT_FAILURE);
	}
}

// Clear the screen using termcap
void term_clear_screen(void)
{
	char *clear_cmd = tgetstr("cl", NULL);
	if (clear_cmd)
		tputs(clear_cmd, 1, putchar);
	else
		fprintf(stderr, "Clear screen capability not supported.\n");
}

// Move the cursor to a specific position
void move_cursor(int row, int col)
{
	char *cursor_cmd = tgetstr("cm", NULL);
	if (cursor_cmd)
		tputs(tgoto(cursor_cmd, col, row), 1, putchar);
	else
		fprintf(stderr, "Cursor movement capability not supported.\n");
}

// Restore the terminal cursor to the starting position
void reset_cursor()
{
	move_cursor(0, 0);
}
