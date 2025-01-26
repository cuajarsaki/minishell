#include "../shell.h"

void	shell_ls(char **args)
{
	char	include_dot;
	char	color;
	char *	path;
	int		arg_counter;

	path = ft_strdup(".");
	include_dot = 0;
	color = 0;
	arg_counter = 0;
	while (args[arg_counter])
	{
		if (args[arg_counter][0] == '-')
		{
			if (ft_strcmp(args[arg_counter], "-a") == 0)
			{
				include_dot = 1;
			}
			else if(ft_strcmp(args[arg_counter], "-color") == 0)
			{
				color = 1;
			}
		}
		else
		{
			path = args[arg_counter];
		}
		arg_counter++;
	}
	DIR *dir;
	struct dirent *entry;

	dir = opendir(path);
	if (dir == NULL)
	{
		perror("ls");
		return;
	}
	int c;
	c = 0;
	while ((entry = readdir(dir)) != NULL)
	{

		if (entry->d_name[0] != '.' || include_dot)
		{
			if (color)
			{
				if (c % 2 == 0)
				{
					write(STDOUT_FILENO, "\033[31m", 5);
					write(STDOUT_FILENO, entry->d_name, ft_strlen(entry->d_name));
					write(STDOUT_FILENO, "\033[0m", 4);
				}
				else
				{
					write(STDOUT_FILENO, entry->d_name, ft_strlen(entry->d_name));
				}
			}
			else
			{
				write(STDOUT_FILENO, entry->d_name, ft_strlen(entry->d_name));
			}
			
			write(STDOUT_FILENO, "\n", 1);
			c++;
		}
	}
	closedir(dir);
}
