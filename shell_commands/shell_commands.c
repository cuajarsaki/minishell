#include "../shell.h"

void shell_unknown_command(char *cmd) {
	write(2, "unknown command: ", 18);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
}

void shell_clear(void)
{
	write(STDOUT_FILENO, "\033[2J\033[H", 7);
}
