#include "shell.h"

void ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void set_prompt(const char *prompt)
{
    int running = 1;

    // Clear screen once at the beginning
    term_clear_screen();

    while (running)
    {
        // Move cursor to the desired position (row 0, column 0)
        move_cursor(0, 0);

        // Print the prompt
        write(STDOUT_FILENO, prompt, strlen(prompt));

        // Handle custom input
        char buf[8192] = {0};
        size_t len = 0;
        handle_input(buf, &len, sizeof(buf));

        // Exit if the buffer is empty or contains "exit"
        if (strcmp(buf, "exit") == 0)
        {
            running = 0;
            continue;
        }

        if (len > 0)
        {
            process_command(buf);

			// HISTORY simulation
            // add_to_history(buf);
        }

        // Optional: Clear the screen after each command
        term_clear_screen();
    }
}


int main()
{
	setup_terminal();
	ignore_signals();
	set_prompt("minishell \u2764 ");
	reset_terminal_settings();

	return 0;
}
