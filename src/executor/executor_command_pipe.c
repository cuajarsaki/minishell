/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_pipe.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 02:11:34 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 02:14:57 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	setup_pipe_and_fork(t_pipe_data *pd, pid_t *pid)
{
	if (pd->i < pd->cmd_count - 1)
	{
		if (pipe(pd->pipe_fd) < 0)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		if (pd->i < pd->cmd_count - 1)
		{
			close(pd->pipe_fd[0]);
			close(pd->pipe_fd[1]);
		}
		exit(EXIT_FAILURE);
	}
}

void	setup_parent_pipe(t_pipe_data *pd)
{
	if (pd->prev_fd != -1)
		close(pd->prev_fd);
	if (pd->i < pd->cmd_count - 1)
	{
		close(pd->pipe_fd[1]);
		pd->prev_fd = pd->pipe_fd[0];
	}
}

void	setup_child_pipe(t_pipe_data pd)
{
	if (pd.prev_fd != -1)
	{
		if (dup2(pd.prev_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pd.prev_fd);
	}
	if (pd.i < pd.cmd_count - 1)
	{
		if (dup2(pd.pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pd.pipe_fd[1]);
		close(pd.pipe_fd[0]);
	}
}
