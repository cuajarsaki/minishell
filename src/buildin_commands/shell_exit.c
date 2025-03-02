/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 16:13:52 by pchung            #+#    #+#             */
/*   Updated: 2025/03/02 19:53:39 by pchung           ###   ########.fr       */
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

// int shell_exit(char **argv , t_saveexit exitstates)
//  wait for issure #5
int shell_exit(char **argv)
{
	long int	res;

	if (argv[1] == NULL)
	{
		printf("exit\n");
        // need to exit last_status
        // wait for issure #5
		exit(0);
	}
	else if (argv[2])
	{
        // when the argument is too many
		perror("exit: too many arguments");
		return (1);
	}
	if (is_strnum(argv[1]))
	{
		res = ft_strtol(argv[1]);
			printf("exit\n");
		exit((int)res);
	}
    // when the argument is not a number
	perror("exit: numeric argument required");
	exit(255);
}
