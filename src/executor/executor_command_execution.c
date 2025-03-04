#include "executor.h"

void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
{
    int fd_in;
    int fd_out;
    int saved_stdin, saved_stdout;
    int exit_status;

    set_filedirectories(cmd, &fd_in, &fd_out);

    // printf("fd_in: %d\n", fd_in);
    // printf("fd_out: %d\n", fd_out);

    save_fds(&saved_stdin, &saved_stdout);
    apply_redirections(fd_in, fd_out);

    if (cmd->tokens != NULL)
    {
        /* Check if built-in */
        if (is_builtin(cmd))
        {
            exit_status = exec_cmd_builtin(cmd, env_list);
            exit(exit_status);           
        }
        else
        {
            // CHILD PROCESS
            char **tokens = convert_list_to_arr(cmd->tokens);
            char *cmd_path = tokens[0];

            // If command has '/' (absolute or relative path), check existence
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
                // Search for the command in PATH
                char *resolved_path = find_executable_in_path(cmd_path);
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
                    exit(127); // Return to parent process
                }
                free(resolved_path);
            }
            free(tokens);
            exec_cmd_external(cmd, command_group, process_index, env_list);
        }
    }
}

int exec_cmd_builtin(t_cmd *cmd, t_env *env_list)
{
    char *program = (char *)cmd->tokens->content;
    char **args = token_list_to_argv(cmd);
    int exit_status;
    // todo:(void)only for temp,need to remove after #5
    (void)exit_status;

    //debug_args(args);

    if (ft_strcmp(program, "echo") == 0)
        exit_status = shell_echo(args);
    else if (ft_strcmp(program, "cd") == 0)
        exit_status = shell_cd(args);
    else if (ft_strcmp(program, "exit") == 0)
        exit_status = shell_exit(args);
    else if (ft_strcmp(program, "pwd") == 0)
        exit_status = shell_pwd();
    else if (ft_strcmp(program, "export") == 0)
        exit_status = shell_export(args, env_list);
    else if (ft_strcmp(program, "unset") == 0)
        exit_status = shell_unset(args, env_list);
    else if (ft_strcmp(program, "env") == 0)
        exit_status = shell_env(env_list); 
    free_argv(args);

    // todo: exit_status for exit status]
    // printf("%d\n", exit_status); //debugs
    return (exit_status);
}

void exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
{
    /* Convert tokens to a NULL-terminated array for execvp */
    char **tokens = convert_list_to_arr(cmd->tokens);

    /* If you do not need env_list here, you can safely ignore it or remove. */
    (void)env_list;
    (void)command_group;
    (void)process_index;

    if (execvp(tokens[0], tokens) == -1)
    {
        perror("execvp");
        free(tokens);       // Free the allocated memory
        exit(EXIT_FAILURE); // Exit the child process if execvp fails
    }
}