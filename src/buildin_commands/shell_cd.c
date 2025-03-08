/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 04:42:38 by pchung           ###   ########.fr       */
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

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		print_cd_error("getcwd");
}

int	shell_cd(char **argv, t_env *env_list)
{
	const char	*path = argv[1];
	char		*home;

	if (path == NULL || strcmp(path, "") == 0)
	{
		home = get_env_value(env_list, "HOME");
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
