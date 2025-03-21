/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:01:57 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 00:57:08 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	free_ast(t_ast *ast)
{
	if (ast)
	{
		ft_lstclear(&ast->command_groups, free);
		free(ast);
	}
}

static void	do_nothing(void *__ptr)
{
	(void)__ptr;
	return ;
}

void	free_command_group(t_command_group *command_group)
{
	if (command_group)
	{
		ft_lstclear(&command_group->cmds, (void (*)(void *))free_cmd);
		ft_lstclear(&command_group->pids, do_nothing);
	}
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
