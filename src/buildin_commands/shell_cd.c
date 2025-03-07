/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/06 12:04:26 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	print_cd_error(const char *msg)
{
	perror(msg);
}

void	print_cd_result(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(1, "Changed directory to: ", 22);
		write(1, cwd, strlen(cwd));
		write(1, "\n", 1);
	}
	else
		print_cd_error("getcwd");
}

int	shell_cd(char **argv)
{
	const char	*path = argv[1];
	char		*home;

	if (path == NULL || strcmp(path, "") == 0)
	{
		home = getenv("HOME");
		if (home == NULL)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
		path = home;
	}
	else if (argv[2])
	{
		write(2, "cd: too many arguments\n", 24);
		return (1);
	}
	if (chdir(path) != 0)
	{
		print_cd_error("cd");
		return (1);
	}
	print_cd_result();
	return (0);
}
