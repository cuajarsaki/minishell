/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:35:07 by pchung            #+#    #+#             */
/*   Updated: 2025/03/08 23:57:01 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	handle_append_output(t_redir *redir, int *fd_out)
{
	if (!redir->direction || *redir->direction == '\0')
	{
		write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n",
			46);
		return (2);
	}
	if (*fd_out != -1)
		close(*fd_out);
	*fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		return (1);
	}
	return (0);
}

static int	handle_heredoc(t_redir *redir, int *fd_in)
{
	if (!redir->direction || *redir->direction == '\0')
	{
		write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n",
			46);
		return (2);
	}
	if (*fd_in != -1)
		close(*fd_in);
	*fd_in = execute_heredoc(redir->direction);
	if (*fd_in == -1)
	{
		perror("heredoc");
		return (1);
	}
	return (0);
}

static int	handle_overwrite_output(t_redir *redir, int *fd_out)
{
	if (!redir->direction || *redir->direction == '\0')
	{
		write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n",
			46);
		return (2);
	}
	if (*fd_out != -1)
		close(*fd_out);
	*fd_out = open(redir->direction, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		return (1);
	}
	return (0);
}

static int	handle_input_redir(t_redir *redir, int *fd_in)
{
	if (!redir->direction || *redir->direction == '\0')
	{
		write(STDERR_FILENO, "syntax error near unexpected token 'newline'\n",
			46);
		return (2);
	}
	if (*fd_in != -1)
		close(*fd_in);
	*fd_in = open(redir->direction, O_RDONLY);
	if (*fd_in == -1)
	{
		perror("open");
		return (1);
	}
	return (0);
}

int	set_filedirectories(t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_list	*redirs;
	t_redir	*redir;
	int		exit_status;

	exit_status = 0;
	if (!cmd)
		return (1);
	redirs = cmd->redirs;
	*fd_in = -1;
	*fd_out = -1;
	while (redirs && !exit_status)
	{
		redir = (t_redir *)redirs->content;
		if (ft_strncmp(redir->type, ">>", 2) == 0)
			exit_status = handle_append_output(redir, fd_out);
		else if (ft_strncmp(redir->type, "<<", 2) == 0)
			exit_status = handle_heredoc(redir, fd_in);
		else if (ft_strncmp(redir->type, ">", 1) == 0)
			exit_status = handle_overwrite_output(redir, fd_out);
		else if (ft_strncmp(redir->type, "<", 1) == 0)
			exit_status = handle_input_redir(redir, fd_in);
		redirs = redirs->next;
	}
	return (exit_status);
}
