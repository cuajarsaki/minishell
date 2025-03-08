/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:01:00 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 03:36:06 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	free_argv(char **argv)
{
	size_t	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_redir(void *ptr)
{
	t_redir	*redir;

	redir = (t_redir *)ptr;
	if (redir)
	{
		if (redir->direction)
			free(redir->direction);
		free(redir);
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		ft_lstclear(&cmd->tokens, free);
		ft_lstclear(&cmd->redirs, free_redir);
		free(cmd);
	}
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
