/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/02/28 12:06:14 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	shell_unknown_command(char *cmd)
{
	write(2, "unknown command: ", 18);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
}

void	shell_clear(void)
{
	write(STDOUT_FILENO, "\033[2J\033[H", 7);
}
