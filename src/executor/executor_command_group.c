/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_group.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:57:12 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 00:13:51 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int exec_single_command(t_command_group *command_group, t_env *env_list,
                              char **envp)
{
	t_cmd	*cmd;
	int		exit_status;
	pid_t	pid;

	exit_status = 0;
	cmd = (t_cmd *)command_group->cmds->content;
	if (is_parent_builtin(cmd))
		exit_status = exec_cmd_builtin(cmd, env_list);
	else
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			init_signal(SIG_DFL, SIG_DFL);
			exec_cmd(cmd, command_group, env_list, envp);
			exit(EXIT_SUCCESS);
		}
		else
		{
			init_signal(SIG_IGN, SIG_IGN);
			ft_lstadd_back(&command_group->pids,ft_lstnew((void *)(intptr_t)pid));
			exit_status = exec_parent(&command_group->pids);
		}
	}
	return (exit_status);
}

static void	setup_child_pipe(int prev_pipe_fd, int i, int cmd_count,
		int pipe_fd[2])
{
	if (prev_pipe_fd != -1)
	{
		if (dup2(prev_pipe_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_fd);
	}
	if (i < cmd_count - 1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
}

static void	setup_parent_pipe(int *prev_pipe_fd, int i, int cmd_count,
		int pipe_fd[2])
{
	if (*prev_pipe_fd != -1)
		close(*prev_pipe_fd);
	if (i < cmd_count - 1)
	{
		close(pipe_fd[1]);
		*prev_pipe_fd = pipe_fd[0];
	}
}

static int exec_multiple_commands(t_command_group *command_group, t_env *env_list,
                                 char **envp)
{
    t_list *cmds;
    int cmd_count;
    int i;
    int pipe_fd[2];
    int prev_pipe_fd;
    int exit_status;
    pid_t pid;

    cmds = command_group->cmds;
    cmd_count = ft_lstsize(cmds);
    i = 0;
    prev_pipe_fd = -1;
    exit_status = 0;
    while (cmds)
    {
        if (i < cmd_count - 1)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork();
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
            init_signal(SIG_DFL, SIG_DFL);
            setup_child_pipe(prev_pipe_fd, i, cmd_count, pipe_fd);
            exec_cmd((t_cmd *)cmds->content, command_group, env_list, envp);
            exit(EXIT_FAILURE);
        }
        else
        {
            init_signal(SIG_IGN, SIG_IGN);
            ft_lstadd_back(&command_group->pids,
                           ft_lstnew((void *)(intptr_t)pid));
            setup_parent_pipe(&prev_pipe_fd, i, cmd_count, pipe_fd);
            cmds = cmds->next;
            i++;
        }
    }
    exit_status = exec_parent(&command_group->pids);
    return (exit_status);
}

int	exec_command_group(t_command_group *command_group, t_env *env_list,
		char **envp)
{
	int	cmd_count;

	if (!command_group)
		return (0);
	cmd_count = ft_lstsize(command_group->cmds);
	if (cmd_count == 0)
		return (0);
	if (cmd_count == 1)
		return (exec_single_command(command_group, env_list, envp));
	else
		return (exec_multiple_commands(command_group, env_list, envp));
}
