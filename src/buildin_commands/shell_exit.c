/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 16:13:52 by pchung            #+#    #+#             */
/*   Updated: 2025/03/06 12:53:38 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static bool	is_strnum(char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (!ft_isdigit(*str))
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	shell_exit(char **argv)
{
	long int	res;

	if (argv[1] == NULL)
	{
		printf("exit\n");
		exit(0);
	}
	else if (argv[2])
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	if (is_strnum(argv[1]))
	{
		res = ft_strtol(argv[1]);
		printf("exit\n");
		exit((int)(res & 0xFF));
	}
	perror("exit: numeric argument required");
	exit(2);
}
