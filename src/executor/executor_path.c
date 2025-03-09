/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:24:32 by pchung            #+#    #+#             */
/*   Updated: 2025/03/08 23:32:07 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*search_executable_in_paths(const char *cmd, char *paths)
{
	char	*dir;
	char	full_path[1024];
	size_t	dir_len;
	size_t	cmd_len;

	dir = ft_strtok(paths, ":");
	while (dir)
	{
		ft_snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
		dir_len = ft_strlen(dir);
		cmd_len = ft_strlen(cmd);
		memcpy(full_path, dir, dir_len);
		full_path[dir_len] = '/';
		memcpy(full_path + dir_len + 1, cmd, cmd_len);
		full_path[dir_len + cmd_len + 1] = '\0';
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_executable_in_path(const char *cmd, t_env *env_list)
{
	char	*path_env;
	char	*paths;
	char	*result;

	path_env = get_env_value(env_list, "PATH");
	if (!path_env || ft_strlen(path_env) == 0)
		return (NULL);
	paths = ft_strdup(path_env);
	result = search_executable_in_paths(cmd, paths);
	free(paths);
	return (result);
}
