/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:45:26 by pchung            #+#    #+#             */
/*   Updated: 2025/02/26 13:45:31 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list);
void exec_command_group(t_command_group *command_group, t_env *env_list);
void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
void exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
void exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int is_builtin(t_cmd *cmd);
void exec_parent(t_list **pids);
char **convert_list_to_arr(t_list *lst);

/* ************************************************************************** */
/*                  🏆 EXECUTE THE ENTIRE AST (MAIN EXECUTION)                */
/* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list)
{
    t_list *command_group_node;
    t_command_group *command_group;
    char *seperator;

    command_group_node = ast->command_groups;
    while (command_group_node)
    {
        command_group = (t_command_group *)command_group_node->content;
        command_group->pids = NULL;
        /* Pass env_list down to the next function */
        exec_command_group(command_group, env_list);

        seperator = command_group->seperator;
        free_command_group(command_group);
        // (Ignoring logical operators for now)
        command_group_node = command_group_node->next;
    }
}

/* ************************************************************************** */
/*                 🏆 EXECUTE A SINGLE COMMAND TABLE                          */
/* ************************************************************************** */

// void exec_command_group(t_command_group *command_group, t_env *env_list)
// {
//     t_list *cmds;
//     int i;

//     cmds = command_group->cmds;
//     command_group->cmd_amount = ft_lstsize(cmds);
//     i = 0;

// 	if (command_group->cmd_amount > 1)
// 	{
// 		printf("Pipe Execution\n");
// 	}

//     while (cmds)
//     {
//         /* Pass env_list into exec_cmd */
//         exec_cmd((t_cmd *)cmds->content, command_group, i, env_list);
//         cmds = cmds->next;
//         i++;
//     }

//     exec_parent(&command_group->pids); // Wait for processes
// }

void exec_command_group(t_command_group *command_group, t_env *env_list)
{
    t_list *cmds = command_group->cmds;
    int cmd_count = ft_lstsize(cmds);
    int i = 0;
    int pipe_fd[2];
    int prev_pipe_fd = -1; // Previous command's output

    if (cmd_count == 0)
        return;

    if (cmd_count == 1)
    {
        // !!!
        // Separate fork handling for cases with and without pipes (Plz notice me if you want to change approach)
        // !!
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // CHILD PROCESS
            exec_cmd((t_cmd *)cmds->content, command_group, i, env_list);
            exit(EXIT_SUCCESS); // Exit the child process
        }
        else
        {
            // PARENT PROCESS
            ft_lstadd_back(&command_group->pids, ft_lstnew((void *)(intptr_t)pid));
            exec_parent(&command_group->pids); // Wait for the child process to complete
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
        exec_parent(&command_group->pids);
    }
}

/* ************************************************************************** */
/*                  🏆 EXECUTE A SINGLE COMMAND                               */
/* ************************************************************************** */

// void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
// {
//     if (cmd->tokens != NULL)
//     {
//         /* Check if built-in */
//         if (is_builtin(cmd)){
// 			printf("Exectuing BuildIN\n");
//             exec_cmd_builtin(cmd, env_list);

// 		}
//         else
// 		{
// 			printf("Exectuing External\n");
//             exec_cmd_external(cmd, command_group, process_index, env_list);
// 		}
//     }
// }

char *find_executable_in_path(const char *cmd)
{
    char *path_env = getenv("PATH");
    if (!path_env || strlen(path_env) == 0)
    {
        return NULL; // No PATH environment variable
    }

    char *paths = strdup(path_env);
    char *dir = strtok(paths, ":");

    while (dir)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return strdup(full_path); // Return a valid command path
        }
        dir = strtok(NULL, ":");
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

    set_filedirectories(cmd, &fd_in, &fd_out);

    printf("fd_in: %d\n", fd_in);
    printf("fd_out: %d\n", fd_out);

    save_fds(&saved_stdin, &saved_stdout);
    apply_redirections(fd_in, fd_out);

    if (cmd->tokens != NULL)
    {
        /* Check if built-in */
        if (is_builtin(cmd))
        {
            exec_cmd_builtin(cmd, env_list);
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
                    fprintf(stderr, "b2ffshell ❤: command not found: %s\n", cmd_path);
                    restore_fds(saved_stdin, saved_stdout);
                    cleanup_fds(fd_in, fd_out);
                    free(tokens);
                    return; // Return to parent process
                }
            }
            else
            {
                // Search for the command in PATH
                char *resolved_path = find_executable_in_path(cmd_path);
                if (!resolved_path)
                {
                    fprintf(stderr, "b2ffshell ❤: command not found: %s\n", cmd_path);
                    restore_fds(saved_stdin, saved_stdout);
                    cleanup_fds(fd_in, fd_out);
                    free(tokens);
                    return; // Return to parent process
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

void exec_cmd_builtin(t_cmd *cmd, t_env *env_list)
{
    char *program = (char *)cmd->tokens->content;
    char **args = convert_list_to_arr(cmd->tokens->next);

    if (strcmp(program, "echo") == 0)
        shell_echo(args);
    else if (strcmp(program, "cd") == 0)
    {
        if (cmd->tokens->next && cmd->tokens->next->content)
            shell_cd((char *)cmd->tokens->next->content);
        else
            shell_cd(NULL);
    }
    else if (strcmp(program, "pwd") == 0)
        shell_pwd();
    else if (strcmp(program, "export") == 0)
        shell_export(args, env_list);
    else if (strcmp(program, "unset") == 0)
        shell_unset(args, env_list);
    else if (strcmp(program, "env") == 0)
        shell_env(env_list);
    free(args);
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

void exec_parent(t_list **pids)
{
    int status;
    t_list *pids_now;

    pids_now = *pids;
    while (pids_now)
    {
        waitpid(-1, &status, 0);
        pids_now = (pids_now)->next;
    }
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
    if (!strcmp(program, "echo") ||
        !strcmp(program, "cd") ||
        !strcmp(program, "pwd") ||
        !strcmp(program, "export") ||
        !strcmp(program, "unset") ||
        !strcmp(program, "env") ||
        !strcmp(program, "exit"))
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
