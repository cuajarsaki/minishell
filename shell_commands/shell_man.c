#include "../shell.h"

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

void	shell_man(char *arg)
{
	int fd;

	if (arg)
	{
		if (strcmp(arg, "ls") == 0)
		{
			fd = open("./commands_manual/ls.txt", O_RDONLY);
		}
		
	}

	
	if (fd > 0)
	{
		puts("\n\n\n--- MANUAL ---\n\n");
		write_fd_to_stdout(fd);
	}
	
	
}