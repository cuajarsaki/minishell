/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:49:12 by pchung            #+#    #+#             */
/*   Updated: 2025/02/26 15:54:59 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void add_to_history(t_history *h, char *command)
{
    if (h->history_count < HISTORY_SIZE)
    {
        h->command_history[h->history_count++] = ft_strdup(command);
    }
    else
    {
        free(h->command_history[h->history_start]);
        h->command_history[h->history_start] = ft_strdup(command);
        h->history_start = (h->history_start + 1) % HISTORY_SIZE;
    }
    h->history_index = h->history_count;
}

const char *get_history(t_history *h, int direction)
{
    int index;

    if (h->history_count == 0)
        return NULL;
    h->history_index += direction;
    if (h->history_index < 0)
        h->history_index = 0;
    else if (h->history_index >= h->history_count)
    {
        h->history_index = h->history_count;
        return "";
    }
    index = (h->history_start + h->history_index) % HISTORY_SIZE;
    return h->command_history[index];
}

void free_history(t_history *h)
{
    int i = 0;

    while (i < h->history_count)
    {
        free(h->command_history[(h->history_start + i) % HISTORY_SIZE]);
        i++;
    }
    free(h);
}

t_history *history_new(void)
{
    t_history *h = malloc(sizeof(t_history));
    if (h)
    {
        h->history_start = 0;
        h->history_count = 0;
        h->history_index = -1;
    }
    return h;
}
