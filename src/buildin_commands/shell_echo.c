/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 02:47:09 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static bool is_n_option(const char *arg)
{
	int i = 0;

	if (!arg || arg[0] != '-')
		return false;
	
	i = 1;
	if (!arg[i])
		return false;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return false;
		i++;
	}
	return true;
}

int shell_echo(char **args)
{
	bool _newline = true;
	int i = 1;

	while (args[i] && is_n_option(args[i]))
	{
		_newline = false;
		i++;
	}

	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}

	if (_newline)
		write(1, "\n", 1);

	return 0;
}