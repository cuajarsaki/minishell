#include "shell.h"
#include <stdlib.h>

#define HISTORY_SIZE 10

char	*command_history[HISTORY_SIZE];
int		history_start = 0;
int		history_count = 0;
int		history_index = -1;

void	add_to_history(char *command)
{
	if (history_count < HISTORY_SIZE)
	{
		command_history[history_count++] = ft_strdup(command);
	}
	else
	{
		free(command_history[history_start]);
		command_history[history_start] = ft_strdup(command);
		history_start = (history_start + 1) % HISTORY_SIZE;
	}
	history_index = history_count;
}

const char	*get_history(int direction)
{
	int	index;

	if (history_count == 0)
		return (NULL);
	history_index += direction;
	if (history_index < 0)
		history_index = 0;
	else if (history_index >= history_count)
	{
		history_index = history_count;
		return ("");
	}
	index = (history_start + history_index) % HISTORY_SIZE;
	return (command_history[index]);
}

void	free_history(void)
{
	int	i;

	i = 0;
	while (i < history_count)
	{
		free(command_history[(history_start + i) % HISTORY_SIZE]);
		i++;
	}
}
