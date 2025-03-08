/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_group.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:57:12 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:46:19 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	exec_single_command(t_command_group *cg, t_env *env_list,
		char **envp)
{
	t_cmd	*cmd;
	int		exit_status;
	pid_t	pid;

	exit_status = 0;
	cmd = (t_cmd *)cg->cmds->content;
	if (is_parent_builtin(cmd))
		return (exec_cmd_builtin(cmd, env_list));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		exec_child_process(cmd, cg, env_list, envp);
	else
	{
		init_signal(SIG_IGN, SIG_IGN);
		ft_lstadd_back(&cg->pids, ft_lstnew((void *)(intptr_t)pid));
		exit_status = exec_parent(&cg->pids);
	}
	return (exit_status);
}

static int	process_command(t_list *cmds, t_command_group *cg, t_env *env_list,
		char **envp)
{
	t_pipe_data	pd;
	pid_t		pid;

	pd.i = 0;
	pd.prev_fd = -1;
	pd.cmd_count = ft_lstsize(cg->cmds);
	pd.cg = cg;
	pd.env_list = env_list;
	pd.envp = envp;
	while (cmds)
	{
		pd.cmd = cmds->content;
		setup_pipe_and_fork(&pd, &pid);
		if (pid == 0)
			execute_child(pd);
		else
		{
			init_signal(SIG_IGN, SIG_IGN);
			ft_lstadd_back(&cg->pids, ft_lstnew((void *)(intptr_t)pid));
			setup_parent_pipe(&pd);
		}
		cmds = cmds->next;
		pd.i++;
	}
	return (EXIT_SUCCESS);
}

static int	exec_multiple_commands(t_command_group *cg, t_env *env_list,
		char **envp)
{
	if (process_command(cg->cmds, cg, env_list, envp) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	return (exec_parent(&cg->pids));
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
