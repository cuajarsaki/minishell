/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_execution1.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:39:13 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 00:41:42 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void exec_cmd(t_cmd *cmd, t_command_group *command_group, t_env *env_list, char **envp)
{
    int fd_in;
    int fd_out;
    int saved_stdin, saved_stdout;
    int exit_status;

    exit_status = set_filedirectories(cmd, &fd_in, &fd_out);

    if(exit_status!=0){
        exit(exit_status);  
    }

    save_fds(&saved_stdin, &saved_stdout);
    apply_redirections(fd_in, fd_out);

    if (cmd->tokens != NULL)
    {
        if (is_builtin(cmd))
        {
            exit_status = exec_cmd_builtin(cmd, env_list);
            exit(exit_status);           
        }
        else
        {
            char **tokens = convert_list_to_arr(cmd->tokens);
            char *cmd_path = tokens[0];

            if (ft_strchr(cmd_path, '/') != NULL)
            {
                if (access(cmd_path, X_OK) == -1)
                {
                    if (access(cmd_path, F_OK) == 0)
                    {
                        exit_status = 126;
                        write(STDERR_FILENO, "minishell ❤", 13);
                        write(STDERR_FILENO, ": ", 2);
                        write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
                        write(STDERR_FILENO, ": Permission denied", 20);
                        write(STDERR_FILENO, "\n", 1);
                    }
                    else
                    {
                        exit_status = 127;
                            write(STDERR_FILENO, "minishell ❤", 13);
                        write(STDERR_FILENO, ": ", 2);
                        write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
                        write(STDERR_FILENO, ": command not found", 19);
                        write(STDERR_FILENO, "\n", 1);
                    }
                    restore_fds(saved_stdin, saved_stdout);
                    cleanup_fds(fd_in, fd_out);
                    free(tokens);
                    exit(exit_status);
                }
            }
            else
            {

                char *resolved_path = find_executable_in_path(cmd_path,env_list);
                if (!resolved_path)
                {

					write(STDERR_FILENO, "minishell ❤", 13);
					write(STDERR_FILENO, ": ", 2);	
					write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
					write(STDERR_FILENO, ": command not found", 19);
					write(STDERR_FILENO, "\n", 1);
						

                    restore_fds(saved_stdin, saved_stdout);
                    cleanup_fds(fd_in, fd_out);
                    free(tokens);
                    exit(127); 
                }
                free(resolved_path);
            }
            free(tokens);
            exit_status = exec_cmd_external(cmd, command_group, env_list, envp);
			exit(exit_status);
        }
    }
}

