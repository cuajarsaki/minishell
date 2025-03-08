/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:09:28 by jidler            #+#    #+#             */
/*   Updated: 2025/03/08 22:41:24 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->tokens = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

void	handle_redirection(t_cmd *cmd, const char *input, int *curr_pos)
{
	t_list	*new_node;

	new_node = ft_lstnew((void *)get_redir(input, curr_pos));
	if (!new_node)
		return ;
	ft_lstadd_back(&cmd->redirs, new_node);
}

void	handle_token(t_cmd *cmd, const char *input,
					int params[2], t_env *env_list)
{
	t_token	*token_struct;
	t_list	*new_node;
	char	*final_token;

	token_struct = get_token(input, &params[0]);
	if (!token_struct)
		return ;
	if (token_struct->is_single_quoted)
		final_token = ft_strdup(token_struct->value);
	else
		final_token = expand_env_token(token_struct->value,
				env_list, &params[1]);
	free(token_struct->value);
	free(token_struct);
	new_node = ft_lstnew((void *)final_token);
	if (!new_node)
	{
		free(final_token);
		return ;
	}
	ft_lstadd_back(&cmd->tokens, new_node);
}

t_cmd	*get_cmd(const char *input, int *curr_pos, t_env *env_list,
		int *exit_status)
{
	t_cmd	*cmd;
	int		params[2];

	params[0] = *curr_pos;
	params[1] = *exit_status;
	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	while (input[params[0]] && input[params[0]] != '|')
	{
		ft_skip_spaces(input, &params[0]);
		if (input[params[0]] == '>' || input[params[0]] == '<')
			handle_redirection(cmd, input, &params[0]);
		else
			handle_token(cmd, input, params, env_list);
	}
	*curr_pos = params[0];
	*exit_status = params[1];
	return (cmd);
}

t_command_group	*get_command_group(const char *input, int *curr_pos,
		t_env *env_list, int *exit_status)
{
	t_command_group	*command_group;
	t_list			*cmd;

	command_group = (t_command_group *)malloc(sizeof(t_command_group));
	if (!command_group)
		return (NULL);
	command_group->cmds = NULL;
	while (input[*curr_pos])
	{
		ft_skip_spaces(input, curr_pos);
		cmd = ft_lstnew((void *)get_cmd(input, curr_pos, env_list,
					exit_status));
		if (!cmd)
		{
			ft_lstclear(&command_group->cmds, (void (*)(void *))free_cmd);
			return (NULL);
		}
		ft_lstadd_back(&command_group->cmds, cmd);
		if (input[*curr_pos] == '|' && input[*curr_pos + 1] != '|')
		{
			(*curr_pos)++;
			continue ;
		}
	}
	return (command_group);
}
