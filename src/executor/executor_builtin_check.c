/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_check.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:51:55 by pchung            #+#    #+#             */
/*   Updated: 2025/03/07 10:24:04 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_builtin(t_cmd *cmd)
{
	char	*program;

	if (!cmd || !cmd->tokens || !cmd->tokens->content)
		return (0);
	program = (char *)cmd->tokens->content;
	if (!ft_strcmp(program, "echo") || !ft_strcmp(program, "cd")
		|| !ft_strcmp(program, "pwd") || !ft_strcmp(program, "export")
		|| !ft_strcmp(program, "unset") || !ft_strcmp(program, "env")
		|| !ft_strcmp(program, "exit"))
	{
		return (1);
	}
	return (0);
}

int	is_parent_builtin(t_cmd *cmd)
{
	char	*command;

	if (!cmd || !cmd->tokens || !cmd->tokens->content)
		return (0);
	command = (char *)cmd->tokens->content;
	return (ft_strcmp(command, "exit") == 0 || ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "unset") == 0 || ft_strcmp(command,
			"export") == 0);
}
