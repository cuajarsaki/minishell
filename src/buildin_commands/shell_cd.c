/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 01:40:37 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int shell_cd(char **argv)
{
	const char *path = argv[1];

	if (path == NULL || ft_strcmp(path, "") == 0)
	{
		char *home = getenv("HOME");
		if (home == NULL)
		{
			write(2, "cd: HOME not set\n", 17);
			return 1;
		}
		path = home;
	}
	else if (argv[2])
	{
		write(2, "cd: too many arguments\n", 24);
		return 1;
	}

	if (chdir(path) != 0)
	{
		perror("cd");
		return 1;
	}
	else
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			write(1, "Changed directory to: ", 22);
			write(1, cwd, ft_strlen(cwd));
			write(1, "\n", 1);
		}
		else
		{
			perror("getcwd");
			return 1;
		}
	}

	return 0;
}