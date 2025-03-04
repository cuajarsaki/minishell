/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:45:26 by pchung            #+#    #+#             */
/*   Updated: 2025/03/04 16:15:20 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

int exec_ast(t_ast *ast, t_env *env_list);
int exec_command_group(t_command_group *command_group, t_env *env_list);
void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
void exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int is_builtin(t_cmd *cmd);
int exec_parent(t_list **pids);
char **convert_list_to_arr(t_list *lst);
int execute_heredoc(const char *delimiter);

    /* ************************************************************************** */
    /*                  🏆 EXECUTE THE ENTIRE AST (MAIN EXECUTION)                */
    /* ************************************************************************** */

int  exec_ast(t_ast *ast, t_env *env_list)
{
    t_list *command_group_node;
    t_command_group *command_group;
    int exit_stauts=0;

    command_group_node = ast->command_groups;
    while (command_group_node)
    {
        command_group = (t_command_group *)command_group_node->content;
        command_group->pids = NULL;
        /* Pass env_list down to the next function */
        exit_stauts = exec_command_group(command_group, env_list);
        free_command_group(command_group);
        // (Ignoring logical operators for now)
        command_group_node = command_group_node->next;
    }
    return(exit_stauts);
}

/* ************************************************************************** */
/*                 🏆 EXECUTE A SINGLE COMMAND TABLE                          */
/* ************************************************************************** */

int exec_command_group(t_command_group *command_group, t_env *env_list)
{
    t_list *cmds = command_group->cmds;
    int cmd_count = ft_lstsize(cmds);
    int i = 0;
    int pipe_fd[2];
    int prev_pipe_fd = -1; // Previous command's output
    int exit_status=0;

    if (cmd_count == 0)
        return(0);

    if (cmd_count == 1)
    {
        t_cmd *cmd = (t_cmd *)cmds->content;
        if (is_builtin(cmd)){
            exit_status = exec_cmd_builtin(cmd, env_list);
        }
        else
        {

                pid_t pid = fork();
                if (pid < 0)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {
                    // CHILD PROCESS
                    init_signal(SIG_DFL, SIG_DFL);
                    exec_cmd((t_cmd *)cmds->content, command_group, i, env_list);
                    exit(EXIT_SUCCESS); // Exit the child process
                }
                else
                {
                    // PARENT PROCESS
                    init_signal(SIG_IGN, SIG_IGN);
                    ft_lstadd_back(&command_group->pids, ft_lstnew((void *)(intptr_t)pid));
                    exit_status = exec_parent(&command_group->pids); // Wait for the child process to complete
                }
        }
    
    }
    else if (cmd_count > 1)
    {
        while (cmds)
        {
            // Create a pipe only if there’s another command after this one
            if (i < cmd_count - 1)
            {
                if (pipe(pipe_fd) < 0)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork");
                if (i < cmd_count - 1)
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                }
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // CHILD PROCESS
                init_signal(SIG_DFL, SIG_DFL);
                // If there's a previous pipe, read from it
                if (prev_pipe_fd != -1)
                {
                    dup2(prev_pipe_fd, STDIN_FILENO);
                    close(prev_pipe_fd);
                }

                // If there's a next command, write output to the pipe
                if (i < cmd_count - 1)
                {
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[1]);
                    close(pipe_fd[0]); // Close unused read-end
                }

                exec_cmd((t_cmd *)cmds->content, command_group, i, env_list);
                exit(EXIT_FAILURE);
            }
            else
            {
                init_signal(SIG_IGN, SIG_IGN);
                // PARENT PROCESS
                ft_lstadd_back(&command_group->pids, ft_lstnew((void *)(intptr_t)pid));

                // Close previous read-end, since it's no longer needed
                if (prev_pipe_fd != -1)
                    close(prev_pipe_fd);

                // Store the new read-end for the next command
                if (i < cmd_count - 1)
                {
                    close(pipe_fd[1]);         // Close write-end in parent
                    prev_pipe_fd = pipe_fd[0]; // Keep read-end for next iteration
                }

                cmds = cmds->next;
                i++;
            }
        }

        // Parent waits for all child processes to complete
        exit_status=exec_parent(&command_group->pids);
    }

    return (exit_status);
}

/* ************************************************************************** */
/*                  🏆 EXECUTE A SINGLE COMMAND                               */
/* ************************************************************************** */

char *find_executable_in_path(const char *cmd)
{
    char *path_env = getenv("PATH");
    if (!path_env || ft_strlen(path_env) == 0)
    {
        return NULL; // No PATH environment variable
    }

    char *paths = ft_strdup(path_env);
    char *dir = ft_strtok(paths, ":");

    while (dir)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

		size_t dir_len = strlen(dir);
		size_t cmd_len = strlen(cmd);

		memcpy(full_path, dir, dir_len);        // Copy `dir`
		full_path[dir_len] = '/';               // Insert '/'
		memcpy(full_path + dir_len + 1, cmd, cmd_len); // Copy `cmd`
		full_path[dir_len + cmd_len + 1] = '\0'; // Null-terminate
		
		if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return ft_strdup(full_path); // Return a valid command path
        }
        dir = ft_strtok(NULL, ":");
    }

    free(paths);
    return NULL;
}

void cleanup_fds(int fd_in, int fd_out)
{
    if (fd_in != -1)
        close(fd_in);
    if (fd_out != -1)
        close(fd_out);
}

void apply_redirections(int fd_in, int fd_out)
{
    if (fd_in != -1)
    {
        if (dup2(fd_in, STDIN_FILENO) == -1) // Redirect input
            perror("dup2 input redirection");
        close(fd_in);
    }
    if (fd_out != -1)
    {
        if (dup2(fd_out, STDOUT_FILENO) == -1) // Redirect output
            perror("dup2 output redirection");
        close(fd_out);
    }
}

void set_filedirectories(t_cmd *cmd, int *fd_in, int *fd_out)
{
    t_list *redirs;

    if (!cmd) // Handle NULL cmd
        return;

    redirs = cmd->redirs;
    *fd_in = -1; // Initialize file descriptors to -1 (unset)
    *fd_out = -1;

    while (redirs)
    {
        t_redir *redir = (t_redir *)redirs->content;

        if (ft_strncmp(redir->type, ">>", 2) == 0) // Append output
        {
            if (*fd_out != -1)
                close(*fd_out);

            *fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*fd_out == -1)
            {
                perror("open");
                return;
            }
        }
        else if (ft_strncmp(redir->type, "<<", 2) == 0) 
        {
            if (*fd_in != -1)
                close(*fd_in);
            *fd_in = execute_heredoc(redir->direction);
            if (*fd_in == -1)
            {
                perror("heredoc");
                return;
            }
        }
        else if (ft_strncmp(redir->type, ">", 1) == 0) // Overwrite output
        {
            if (*fd_out != -1) // Close previous output redirection
                close(*fd_out);

            *fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*fd_out == -1)
            {
                perror("open");
                return;
            }
        }
        else if (ft_strncmp(redir->type, "<", 1) == 0) // Input redirection
        {
            if (*fd_in != -1)
                close(*fd_in);

            *fd_in = open(redir->direction, O_RDONLY);
            if (*fd_in == -1)
            {
                perror("open");
                return;
            }
        }

        redirs = redirs->next;
    }
}

int execute_heredoc(const char *delimiter)
{
    const char *filename = "/tmp/minishell_heredoc";
    int fd;

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
    {
        perror("open heredoc for writing");
        return -1;
    }

    char *line = NULL;
    while (1)
    {
        line = readline("> ");
        if (!line){
            free(line);
            break;
        }
        
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open heredoc for reading");
        return -1;
    }
    
    unlink(filename);
    return fd;
}

void save_fds(int *saved_stdin, int *saved_stdout)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
        perror("save_fds: dup failed");
}

void restore_fds(int saved_stdin, int saved_stdout)
{
    if (dup2(saved_stdin, STDIN_FILENO) == -1)
        perror("restore_fds: dup2 stdin failed");
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
        perror("restore_fds: dup2 stdout failed");

    close(saved_stdin);
    close(saved_stdout);
}

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

/* ************************************************************************** */
/*              🏆 EXECUTE BUILT-IN COMMANDS (NO FORKING REQUIRED)            */
/* ************************************************************************** */

// todo:this is for debug only ,remove it later
void debug_args(char **args)
{
    if (!args)
    {
        printf("debug_args: args is NULL\n");
        return;
    }

    printf("=== Debugging args ===\n");
    for (int i = 0; args[i] != NULL; i++)
    {
        printf("args[%d] = \"%s\"\n", i, args[i]);
    }
    printf("======================\n");
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

/* ************************************************************************** */
/*             🏆 EXECUTE EXTERNAL PROGRAMS (REQUIRES FORKING)                */
/* ************************************************************************** */

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

int ft_execvp(const char *file, char *const argv[])
{
    char *path_env = getenv("PATH");
    char **paths = NULL;
    char *cmd_path;
    int i = 0;

    if (!file || !*file)
    {
        errno = ENOENT;
        return (-1);
    }
    if (has_slash(file))                   // Instead of strchr(file, '/')
        return (execve(file, argv, NULL)); // Execute directly if it's an absolute/relative path
    if (path_env)
        paths = ft_split(path_env, ':');
    while (paths && paths[i])
    {
        cmd_path = ft_strjoin(paths[i], file);
        if (access(cmd_path, X_OK) == 0)
        {
            execve(cmd_path, argv, NULL);
            free(cmd_path);
            break;
        }
        free(cmd_path);
        i++;
    }
    free(paths);
    errno = ENOENT;
    return (-1);
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

/* ************************************************************************** */
/*              🏆 WAIT FOR ALL CHILD PROCESSES TO FINISH                     */
/* ************************************************************************** */


int exec_parent(t_list **pids)
{
    int status;
    t_list *pids_now;

    pids_now = *pids;
    while (pids_now)
    {
        pid_t pid = (pid_t)(intptr_t)pids_now->content;
        waitpid(pid, &status, 0);
        pids_now = pids_now->next;
        
    }
    // // debugs
    // printf("debugs:(in exec_parent)\n");
    // // debugs
    // printf("debugs:Exit status before WIFEXITED: %d\n", status);
    // if (WIFEXITED(status))
    //     // debugs
    //     printf("debugs:Exit status: %d\n", WEXITSTATUS(status));

    // TODO:change WEXITSTATUS later!
    return (WEXITSTATUS(status));
}

/* ************************************************************************** */
/*              🏆 HELPER FUNCTIONS FOR COMMAND EXECUTION                     */
/* ************************************************************************** */

/* Check if command is a built-in */
int is_builtin(t_cmd *cmd)
{
    char *program = (char *)cmd->tokens->content;

    /*
        NOTE: Currently, this returns '0' for these commands,
        meaning "not recognized as builtin." If you want them
        recognized as built-ins, return 1 for matches instead.
    */
    if (!ft_strcmp(program, "echo") ||
        !ft_strcmp(program, "cd") ||
        !ft_strcmp(program, "pwd") ||
        !ft_strcmp(program, "export") ||
        !ft_strcmp(program, "unset") ||
        !ft_strcmp(program, "env") ||
        !ft_strcmp(program, "exit"))
    {
        return 1; // <--- This effectively says "Not builtin" in your logic
    }
    return 0; // <--- Everything else also returns "Not builtin"
}

/* Convert linked list to a NULL-terminated C-string array */
char **convert_list_to_arr(t_list *lst)
{
    int len = ft_lstsize(lst);
    char **arr = (char **)malloc(sizeof(char *) * (len + 1));
    int i = 0;

    while (lst)
    {
        arr[i++] = (char *)lst->content;
        lst = lst->next;
    }
    arr[i] = NULL;
    return arr;
}
