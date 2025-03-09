/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:35:46 by pchung            #+#    #+#             */
/*   Updated: 2025/03/08 23:45:55 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**convert_list_to_arr(t_list *lst)
{
	int		len;
	char	**arr;
	int		i;

	len = ft_lstsize(lst);
	arr = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (lst)
	{
		arr[i++] = (char *)lst->content;
		lst = lst->next;
	}
	arr[i] = NULL;
	return (arr);
}

static void	heredoc_loop(const char *delimiter, int fd)
{
	char	*line;
	int		line_number;

	line = NULL;
	line_number = 1;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document at line%d", line_number);
			printf(" delimited by end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line_number++;
	}
}

int	execute_heredoc(const char *delimiter)
{
	char	*filename;
	int		fd;

	filename = "/tmp/minishell_heredoc";
	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("open heredoc for writing");
		return (-1);
	}
	heredoc_loop(delimiter, fd);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open heredoc for reading");
		return (-1);
	}
	unlink(filename);
	return (fd);
}
