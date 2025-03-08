/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:34:02 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:46:37 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	exec_ast(t_ast *ast, t_env *env_list, char **envp)
{
	t_list			*command_group_node;
	t_command_group	*command_group;
	int				exit_stauts;

	exit_stauts = 0;
	command_group_node = ast->command_groups;
	while (command_group_node)
	{
		command_group = (t_command_group *)command_group_node->content;
		command_group->pids = NULL;
		exit_stauts = exec_command_group(command_group, env_list, envp);
		free_command_group(command_group);
		command_group_node = command_group_node->next;
	}
	return (exit_stauts);
}
