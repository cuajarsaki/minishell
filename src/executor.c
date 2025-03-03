/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:45:26 by pchung            #+#    #+#             */
/*   Updated: 2025/03/02 16:4 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list, int *exit_status);
void exec_command_group(t_command_group *command_group, t_env *env_list, int *exit_status);

void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list, int *exit_status);
void exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
int exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int is_builtin(t_cmd *cmd);
int exec_parent(t_list **pids);
char **convert_list_to_arr(t_list *lst);
int execute_heredoc(const char *delimiter);

extern char **environ;


    /* ************************************************************************** */
    /*                  🏆 EXECUTE THE ENTIRE AST (MAIN EXECUTION)                */
    /* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list, int *exit_status)
{
    t_list *command_group_node;
    t_command_group *command_group;

    command_group_node = ast->command_groups;
    while (command_group_node)
    {
        command_group = (t_command_group *)command_group_node->content;
        command_group->pids = NULL;
        /* Pass env_list down to the next function */
        exec_command_group(command_group, env_list, exit_status);
        free_command_group(command_group);
        // (Ignoring logical operators for now)
        command_group_node = command_group_node->next;
    }
}

/* ************************************************************************** */
/*                 🏆 EXECUTE A SINGLE COMMAND TABLE                          */
/* ************************************************************************** */


void exec_command_group(t_command_group *command_group, t_env *env_list, int *exit_status)
{
    t_list *cmds = command_group->cmds;
    int cmd_count = ft_lstsize(cmds);
    int i = 0;
    int pipe_fd[2];
    int prev_pipe_fd = -1;

    if (cmd_count == 0)
        return;

    while (cmds)
    {
        if (i < cmd_count - 1 && pipe(pipe_fd) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            init_signal(SIG_DFL, SIG_DFL);
            if (prev_pipe_fd != -1) {
                dup2(prev_pipe_fd, STDIN_FILENO);
                close(prev_pipe_fd);
            }
            if (i < cmd_count - 1) {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
                close(pipe_fd[0]);
            }

            exec_cmd((t_cmd *)cmds->content, command_group, i, env_list, exit_status);
            exit(*exit_status);
        }
        else {
            init_signal(SIG_IGN, SIG_IGN);
            ft_lstadd_back(&command_group->pids, ft_lstnew((void *)(intptr_t)pid));
            if (prev_pipe_fd != -1)
                close(prev_pipe_fd);
            if (i < cmd_count - 1) {
                close(pipe_fd[1]);
                prev_pipe_fd = pipe_fd[0];
            }
            cmds = cmds->next;
            i++;
        }
    }

    *exit_status = exec_parent(&command_group->pids);
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


void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list, int *exit_status)
{
    int fd_in, fd_out, saved_stdin, saved_stdout;

    set_filedirectories(cmd, &fd_in, &fd_out);
    save_fds(&saved_stdin, &saved_stdout);
    apply_redirections(fd_in, fd_out);

    if (cmd->tokens != NULL) {
        if (is_builtin(cmd)) {
            exec_cmd_builtin(cmd, env_list);
            *exit_status = 0;  // Builtins usually succeed unless they explicitly return an error
        } else {
            *exit_status = exec_cmd_external(cmd, command_group, process_index, env_list);
        }
    }

    restore_fds(saved_stdin, saved_stdout);
}


/* ************************************************************************** */
/*              🏆 EXECUTE BUILT-IN COMMANDS (NO FORKING REQUIRED)            */
/* ************************************************************************** */

void exec_cmd_builtin(t_cmd *cmd, t_env *env_list)
{
    char *program = (char *)cmd->tokens->content;
    char **args = convert_list_to_arr(cmd->tokens->next);

    if (ft_strcmp(program, "echo") == 0)
        shell_echo(args);
    else if (ft_strcmp(program, "cd") == 0)
    {
        if (cmd->tokens->next && cmd->tokens->next->content)
            shell_cd((char *)cmd->tokens->next->content);
        else
            shell_cd(NULL);
    }
    else if (ft_strcmp(program, "pwd") == 0)
        shell_pwd();
    else if (ft_strcmp(program, "export") == 0)
        shell_export(args, env_list);
    else if (ft_strcmp(program, "unset") == 0)
        shell_unset(args, env_list);
    else if (ft_strcmp(program, "env") == 0)
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

/* Helper function to check if a command contains a '/' */

char *ft_strjoin2(const char *path, const char *cmd) {
    size_t len1 = strlen(path);
    size_t len2 = strlen(cmd);
    char *full_path = malloc(len1 + len2 + 2); // +2 for '/' and '\0'

    if (!full_path)
        return NULL;

    strcpy(full_path, path);

    // Ensure there is a '/' between path and command
    full_path[len1] = '/';

    strcpy(full_path + len1 + 1, cmd);
    full_path[len1 + len2 + 1] = '\0'; // Null-terminate

    return full_path;
}


/* Custom execvp using only execve */
int ft_execvp(const char *file, char *const argv[]) {
    char *path_env = getenv("PATH");
    char **paths = NULL;
    char *cmd_path;
    int i = 0;

    if (!file || !*file) {
		//return global error
		return -1;
    }

    /* Print PATH for debugging */
    printf("PATH: %s\n", path_env);

    /* If command contains '/' execute it directly */
    if (has_slash(file)) {
        printf("Executing directly: %s\n", file);
        return execve(file, argv, environ);
    }

    /* Split PATH environment variable */
    if (path_env)
        paths = ft_split(path_env, ':');

    while (paths && paths[i]) {
        cmd_path = ft_strjoin2(paths[i], file);
        printf("Checking: %s\n", cmd_path);  // Debugging output
        if (cmd_path && access(cmd_path, X_OK) == 0) {
            execve(cmd_path, argv, environ);
            free(cmd_path);
            break;
        }
        free(cmd_path);
        i++;
    }

    /* Cleanup */
    for (i = 0; paths && paths[i]; i++)
        free(paths[i]);
    free(paths);


    return -1;
}


#include <sys/types.h>
#include <sys/wait.h>

int exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
{
    char **tokens = convert_list_to_arr(cmd->tokens);
    pid_t pid;
    int status;

    (void)env_list;
    (void)command_group;
    (void)process_index;

    pid = fork();

    if (pid == 0) { // Child process
        ft_execvp(tokens[0], tokens);
        
        // If execvp fails, print debugging information
        perror("Execvp failed");
        
        for (int i = 0; tokens[i]; i++)
            free(tokens[i]);
        free(tokens);

        exit(127); // 127 is commonly used for command not found
    } 
    else if (pid < 0) { // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // Parent process waits for the child and retrieves its exit status
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    // Free tokens after execution
    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);

    // Return the exit status
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } 
    else if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status); // Signal exit codes start from 128
    } 
    return 1; // Default error
}


/* ************************************************************************** */
/*              🏆 WAIT FOR ALL CHILD PROCESSES TO FINISH                     */
/* ************************************************************************** */

int exec_parent(t_list **pids)
{
    int status = 0;
    t_list *pids_now = *pids;

    while (pids_now)
    {
        if (!pids_now->next)
            waitpid((pid_t)(intptr_t)pids_now->content, &status, 0);
        else
            waitpid((pid_t)(intptr_t)pids_now->content, NULL, 0);
        pids_now = pids_now->next;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1; // Default error
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
