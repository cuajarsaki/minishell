#include "../shell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void write_fd_to_stdout(int fd)
{
	char buffer[42];
	ssize_t bytes_read;

	while ((bytes_read = read(fd, buffer, 42)) > 0) {
		if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
			perror("write");
			return;
		}
	}
	if (bytes_read == -1) {
		perror("read");
	}
}

void shell_man(char *arg)
{
	int fd = -1;

	if (arg)
	{
		if (ft_strcmp(arg, "ls") == 0) {
			fd = open("./commands_manual/ls.txt", O_RDONLY);
		}
		else if (ft_strcmp(arg, "cd") == 0) {
			fd = open("./commands_manual/cd.txt", O_RDONLY);
		}
		else if (ft_strcmp(arg, "echo") == 0) {
			fd = open("./commands_manual/echo.txt", O_RDONLY);
		}
		else {
			write(STDERR_FILENO, "No manual entry for ", 20);
			write(STDERR_FILENO, arg, ft_strlen(arg));
			write(STDERR_FILENO, "\n", 1);
		}
	}
	else {
		write(STDERR_FILENO, "Usage: man <command>\n", 22);
	}
	if (fd > 0) {
		write(STDOUT_FILENO, "\n\n\n--- MANUAL ---\n\n", 21);
		write_fd_to_stdout(fd);
		close(fd);
	} else if (fd == -1 && arg && ft_strcmp(arg, "ls") == 0) {
		perror("open");
	}
}
