/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_buildin.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 02:21:13 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:32:18 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	process_builtin(t_cmd *cmd, t_exec_ctx *ctx)
{
	int	exit_status;

	exit_status = exec_cmd_builtin(cmd, ctx->env_list);
	restore_fds(ctx->saved_stdin, ctx->saved_stdout);
	cleanup_fds(ctx->fd_in, ctx->fd_out);
	exit(exit_status);
}

void	exec_non_builtin(t_cmd *cmd, t_exec_ctx *ctx)
{
	char	**tokens;
	char	*cmd_path;
	int		exit_status;

	tokens = convert_list_to_arr(cmd->tokens);
	cmd_path = tokens[0];
	if (ft_strchr(cmd_path, '/') != NULL)
		handle_cmd_with_path(cmd_path, ctx, tokens);
	else
		handle_cmd_without_path(cmd_path, ctx, tokens);
	free(tokens);
	exit_status = exec_cmd_external(cmd, ctx->cmd_group, ctx->env_list,
			ctx->envp);
	exit(exit_status);
}
