/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_execution2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:41:03 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:00:50 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	has_slash(const char *str)
{
	while (*str)
	{
		if (*str == '/')
			return (1);
		str++;
	}
	return (0);
}

char	*ft_strjoin2(const char *path, const char *cmd)
{
	size_t	len1;
	size_t	len2;
	char	*full_path;

	len1 = strlen(path);
	len2 = strlen(cmd);
	full_path = malloc(len1 + len2 + 2);
	if (!full_path)
		return (NULL);
	strcpy(full_path, path);
	full_path[len1] = '/';
	strcpy(full_path + len1 + 1, cmd);
	full_path[len1 + len2 + 1] = '\0';
	return (full_path);
}

static int	try_execve_in_paths(const char *file, char *const argv[],
		char **envp, char **paths)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (paths && paths[i])
	{
		cmd_path = ft_strjoin2(paths[i], file);
		if (cmd_path && access(cmd_path, X_OK) == 0)
		{
			execve(cmd_path, argv, envp);
			free(cmd_path);
			return (0);
		}
		free(cmd_path);
		i++;
	}
	return (-1);
}

int	ft_execvp(const char *file, char *const argv[], char **envp,
		t_env *env_list)
{
	char	*path_env;
	char	**paths;

	path_env = get_env_value(env_list, "PATH");
	paths = NULL;
	if (!file || !*file)
		return (-1);
	if (has_slash(file))
	{
		printf("Executing directly: %s\n", file);
		return (execve(file, argv, envp));
	}
	if (path_env)
		paths = ft_split(path_env, ':');
	if (try_execve_in_paths(file, argv, envp, paths) == 0)
	{
		free_paths(paths);
		return (0);
	}
	free_paths(paths);
	return (-1);
}

int	exec_cmd_external(t_cmd *cmd, t_command_group *command_group,
		t_env *env_list, char **envp)
{
	char	**tokens;
	int		i;

	(void)command_group;
	tokens = convert_list_to_arr(cmd->tokens);
	ft_execvp(tokens[0], tokens, envp, env_list);
	perror("ft_execvp failed");
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	exit(127);
}
