/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 00:08:24 by pchung            #+#    #+#             */
/*   Updated: 2025/03/05 23:07:53 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"


void	exit_program()
{
	exit(0);
}


void handle_input(char *buf, size_t *len, size_t buffsize) {
    g_signal_received = NOTSIG;
    char *str;   
    str = readline("minishell ❤ ");
    if (str == NULL)
    {
        write(STDOUT_FILENO, "exit\n", 5);
        exit(0);
    }

    add_history(str);
    ft_strlcpy(buf, str, buffsize); 
    *len = ft_strlen(buf);
    free(str);
}