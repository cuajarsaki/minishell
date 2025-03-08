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

int exec_cmd_builtin(t_cmd *cmd, t_env *env_list)
{
    char *program = (char *)cmd->tokens->content;
    char **args = token_list_to_argv(cmd);
    int exit_status = 0;

    if (ft_strcmp(program, "echo") == 0)
        exit_status = shell_echo(args);
    else if (ft_strcmp(program, "cd") == 0)
        exit_status = shell_cd(args, env_list);
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

    return (exit_status);
}

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




char *ft_strjoin2(const char *path, const char *cmd) {
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

int ft_execvp(const char *file, char *const argv[], char **envp,t_env *env_list)
{
    char *path_env = get_env_value(env_list, "PATH");
    char **paths = NULL;
    char *cmd_path;
    int i = 0;

    if (!file || !*file) 
        return -1;

    if (has_slash(file)) {
        printf("Executing directly: %s\n", file);
        return execve(file, argv, envp); 
    }

    if (path_env)
        paths = ft_split(path_env, ':');

    while (paths && paths[i]) {
        cmd_path = ft_strjoin2(paths[i], file);
        if (cmd_path && access(cmd_path, X_OK) == 0) {
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

    if (pid == 0) { 
        ft_execvp(tokens[0], tokens, envp, env_list);

        perror("Execvp failed");
        
        for (int i = 0; tokens[i]; i++)
            free(tokens[i]);
        free(tokens);

        exit(127); 
    } 
    else if (pid < 0) { 
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);

    if ((status & 0x7F) == 0) {
        return (status >> 8) & 0xFF;
    } else if (status & 0x7F) {
        return 128 + (status & 0x7F);
    }
    return 1;
}

