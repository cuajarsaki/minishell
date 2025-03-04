#include "parser.h"

t_cmd	*get_cmd(const char *input, int *curr_pos, t_env *env_list, int *exit_status)
{
	t_cmd	*cmd;
	t_list	*new_node;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->tokens = NULL;
	cmd->redirs = NULL;

	while (input[*curr_pos] && input[*curr_pos] != '|')
	{
		ft_skip_spaces(input, curr_pos);

		if (input[*curr_pos] == '>' || input[*curr_pos] == '<')
		{
			new_node = ft_lstnew((void *)get_redir(input, curr_pos));
			if (!new_node)
				return (NULL);
			ft_lstadd_back(&cmd->redirs, new_node);
		}
		else
		{
			t_token *token_struct = get_token(input, curr_pos);
			if (token_struct)
			{
				char *final_token = token_struct->is_single_quoted ? ft_strdup(token_struct->value) :
					expand_env_token(token_struct->value, env_list, exit_status);

				free(token_struct->value);
				free(token_struct);

				new_node = ft_lstnew((void *)final_token);
				if (!new_node)
				{
					free(final_token);
					return (NULL);
				}
				ft_lstadd_back(&cmd->tokens, new_node);
			}
		}	
	}
	return (cmd);
}

t_command_group	*get_command_group(const char *input, int *curr_pos, t_env *env_list, int *exit_status)
{
	t_command_group	*command_group;
	t_list		*cmd;

	command_group = (t_command_group *)malloc(sizeof(t_command_group));
	if (!command_group)
		return (NULL);
	command_group->cmds = NULL;

	while (input[*curr_pos])
	{
		ft_skip_spaces(input, curr_pos);

		// Parse commands inside this table
		cmd = ft_lstnew((void *)get_cmd(input, curr_pos, env_list, exit_status));
		if (!cmd)
		{
			ft_lstclear(&command_group->cmds, (void (*)(void *))free_cmd);
			return (NULL);
		}
		ft_lstadd_back(&command_group->cmds, cmd);

		// Handle pipes within the same command table
		if (input[*curr_pos] == '|' && input[*curr_pos + 1] != '|')
		{
			(*curr_pos)++; // Skip the `|`
			continue; // Stay inside the same `t_command_group`
		}
	}
	return (command_group);
}