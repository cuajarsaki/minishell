#include "executor.h"

int exec_command_group(t_command_group *command_group, t_env *env_list, char **envp)
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
        if ((cmd->tokens) && (cmd->tokens->content) &&(ft_strcmp((char *)cmd->tokens->content, "exit") == 0 ||
     ft_strcmp((char *)cmd->tokens->content, "cd") == 0))
{
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
                    exec_cmd((t_cmd *)cmds->content, command_group, i, env_list, envp);
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

                exec_cmd((t_cmd *)cmds->content, command_group, i, env_list, envp);
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