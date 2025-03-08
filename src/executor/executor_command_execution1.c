/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command_execution1.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:39:13 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:55:08 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	print_error_and_exit(const char *cmd_path, int exit_status,
		t_exec_ctx *ctx, char **tokens)
{
	write(STDERR_FILENO, "minishell ❤", 13);
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
	if (exit_status == 126)
		write(STDERR_FILENO, ": Permission denied", 20);
	else
		write(STDERR_FILENO, ": command not found", 19);
	write(STDERR_FILENO, "\n", 1);
	restore_fds(ctx->saved_stdin, ctx->saved_stdout);
	cleanup_fds(ctx->fd_in, ctx->fd_out);
	free_tokens(tokens);
	exit(exit_status);
}

int	check_executable_access(char *cmd_path, t_exec_ctx *ctx,
		char **tokens)
{
	int	exit_status;

	if (access(cmd_path, F_OK) == 0)
		exit_status = 126;
	else
		exit_status = 127;
	print_error_and_exit(cmd_path, exit_status, ctx, tokens);
	return (exit_status);
}

static void	setup_context(t_exec_ctx *ctx,
		t_command_group *command_group, t_env *env_list)
{
	ctx->cmd_group = command_group;
	ctx->env_list = env_list;
}

static void	process_command(t_cmd *cmd, t_exec_ctx *ctx)
{
	if (is_builtin(cmd))
		process_builtin(cmd, ctx);
	else
		exec_non_builtin(cmd, ctx);
}

void	exec_cmd(t_cmd *cmd, t_command_group *command_group, t_env *env_list,
		char **envp)
{
	t_exec_ctx	ctx;
	int			fd_in;
	int			fd_out;
	int			saved_std[2];
	int			exit_status;

	exit_status = set_filedirectories(cmd, &fd_in, &fd_out);
	if (exit_status != 0)
		exit(exit_status);
	save_fds(&(saved_std[0]), &(saved_std[1]));
	apply_redirections(fd_in, fd_out);
	if (cmd->tokens != NULL)
	{
		setup_context(&ctx, command_group, env_list);
		ctx.envp = envp;
		ctx.saved_stdin = saved_std[0];
		ctx.saved_stdout = saved_std[1];
		ctx.fd_in = fd_in;
		ctx.fd_out = fd_out;
		process_command(cmd, &ctx);
	}
}
