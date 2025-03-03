/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 00:49:51 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int shell_cd(char *path)
{
	if (path == NULL || ft_strcmp(path, "") == 0) {
		char *home = getenv("HOME");
		if (home == NULL) {
			write(2, "cd: HOME not set\n", 17);
			return 1;
		}
		path = home;
	}
	if (chdir(path) != 0) {
		perror("cd");
	} else {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			write(1, "Changed directory to: ", 22);
			write(1, cwd, ft_strlen(cwd));
			write(1, "\n", 1);
		} else {
			perror("getcwd");
			return 1;
		}
	}
	return 0;
}