/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:35:46 by pchung            #+#    #+#             */
/*   Updated: 2025/03/05 09:35:47 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char **convert_list_to_arr(t_list *lst)
{
    int len = ft_lstsize(lst);
    char **arr = (char **)malloc(sizeof(char *) * (len + 1));
    int i = 0;

    while (lst)
    {
        arr[i++] = (char *)lst->content;
        lst = lst->next;
    }
    arr[i] = NULL;
    return arr;
}


int execute_heredoc(const char *delimiter)
{
    const char *filename = "/tmp/minishell_heredoc";
    int fd;

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
    {
        perror("open heredoc for writing");
        return -1;
    }

    char *line = NULL;
    int line_number = 1;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", 
                   line_number, delimiter);
            break;
        }

        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        line_number++;
    }
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open heredoc for reading");
        return -1;
    }
    
    unlink(filename);
    return fd;
}