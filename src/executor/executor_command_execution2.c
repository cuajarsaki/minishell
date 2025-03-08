/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_execution2.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:41:03 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 00:42:15 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int has_slash(const char *str)
{
    while (*str)
    {
        if (*str == '/')
            return (1);
        str++;
    }
    return (0);
}

char *ft_strjoin2(const char *path, const char *cmd)
{
    size_t len1 = strlen(path);
    size_t len2 = strlen(cmd);
    char *full_path = malloc(len1 + len2 + 2);

    if (!full_path)
        return NULL;

    strcpy(full_path, path);

    full_path[len1] = '/';

    strcpy(full_path + len1 + 1, cmd);
    full_path[len1 + len2 + 1] = '\0';

    return full_path;
}

int ft_execvp(const char *file, char *const argv[], char **envp, t_env *env_list)
{
    char *path_env = get_env_value(env_list, "PATH");
    char **paths = NULL;
    char *cmd_path;
    int i = 0;

    if (!file || !*file)
        return -1;

    if (has_slash(file))
    {
        printf("Executing directly: %s\n", file);
        return execve(file, argv, envp);
    }

    if (path_env)
        paths = ft_split(path_env, ':');

    while (paths && paths[i])
    {
        cmd_path = ft_strjoin2(paths[i], file);
        if (cmd_path && access(cmd_path, X_OK) == 0)
        {
            execve(cmd_path, argv, envp);
            free(cmd_path);
            break;
        }
        free(cmd_path);
        i++;
    }

    for (i = 0; paths && paths[i]; i++)
        free(paths[i]);
    free(paths);

    return -1;
}

int exec_cmd_external(t_cmd *cmd, t_command_group *command_group, t_env *env_list, char **envp)
{
    char **tokens = convert_list_to_arr(cmd->tokens);
    pid_t pid;
    int status;

    (void)env_list;
    (void)command_group;
    pid = fork();

    if (pid == 0)
    {
        ft_execvp(tokens[0], tokens, envp, env_list);

        perror("Execvp failed");

        for (int i = 0; tokens[i]; i++)
            free(tokens[i]);
        free(tokens);

        exit(127);
    }
    else if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);

    if ((status & 0x7F) == 0)
    {
        return (status >> 8) & 0xFF;
    }
    else if (status & 0x7F)
    {
        return 128 + (status & 0x7F);
    }
    return 1;
}
