#include "../shell.h"

void shell_echo(char **args)
{
	// compile error occurs if dont comment out this line?
	// need to be double check ��
	// bool newline = true;
	int i = 0;
	
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}
