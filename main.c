#include "shell.h"

void ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void set_prompt(const char *prompt)
{
	int again = 1;
	while (again)
	{
		again = 0;
		char *input = readline(prompt);
		if (input == NULL || strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		if (*input) {
			// puts("Received Input: ");
			// puts(input);
			process_command(input);
			add_history(input);
		}
		free(input);
		again = 1;
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
