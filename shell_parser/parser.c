/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:44:55 by jidler            #+#    #+#             */
/*   Updated: 2025/02/09 16:25:44 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"
#include <ctype.h>

/* ************************************************************************** */
/*                            UTILITY FUNCTIONS                               */
/* ************************************************************************** */

void	ft_skip_spaces(const char *input, int *curr_pos)
{
	while (input[*curr_pos] && isspace(input[*curr_pos]))
		(*curr_pos)++;
}

int		is_cmd_seperator(char c)
{
	return (c == ';' || c == '&');
}

int		is_command_group_seperator(const char *str)
{
	return (strncmp(str, "||", 2) == 0 || strncmp(str, "&&", 2) == 0 || *str == ';');
}

char	*get_command_group_seperator(const char *input, int *curr_pos)
{
	if (input[*curr_pos] == ';')
	{
		(*curr_pos)++;
		return (strdup(";"));
	}
	else if (strncmp(&input[*curr_pos], "||", 2) == 0)
	{
		*curr_pos += 2;
		return (strdup("||"));
	}
	else if (strncmp(&input[*curr_pos], "&&", 2) == 0)
	{
		*curr_pos += 2;
		return (strdup("&&"));
	}
	return (strdup(""));
}

static char *expand_env_token(const char *token, t_env *env_list)
{
    // 1) If token is NULL or empty, just return a copy
    if (!token || !*token)
        return (token ? strdup(token) : NULL);

    // 2) If token does NOT start with '$', return a copy unchanged
    if (token[0] != '$')
        return strdup(token);

    // 3) Extract the variable name (everything after '$')
    const char *var_name = token + 1;

    // Edge case: if the token is just "$", no var_name
    if (!*var_name)
        return strdup("$"); // or return strdup("") depending on your shell’s behavior

    // 4) Look up var_name in env_list
    char *val = get_env_value(env_list, var_name);

    // If not found, some shells use an empty string
    if (!val)
        val = "";

    // 5) Return a newly allocated copy of the expansion
    return strdup(val);
}

/* ************************************************************************** */
/*                           PARSING FUNCTIONS                                */
/* ************************************************************************** */

t_redir	*get_redir(const char *input, int *curr_pos)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);

	// Identify redirection type
	if (strncmp(&input[*curr_pos], ">>", 2) == 0)
	{
		strcpy(redir->type, ">>");
		*curr_pos += 2;
	}
	else
	{
		redir->type[0] = input[(*curr_pos)++];
		redir->type[1] = '\0';
	}

	ft_skip_spaces(input, curr_pos);

	// Extract redirection target (filename)
	int start = *curr_pos;
	while (input[*curr_pos] && !isspace(input[*curr_pos]) && !is_cmd_seperator(input[*curr_pos]) && input[*curr_pos] != '|')
		(*curr_pos)++;

	redir->direction = strndup(&input[start], *curr_pos - start);
	return (redir);
}

char	*get_token(const char *input, int *curr_pos)
{
	ft_skip_spaces(input, curr_pos); // Skip leading spaces before reading a token
	int start = *curr_pos;

	// Extract token until space, command seperator, or redirection
	while (input[*curr_pos] && !isspace(input[*curr_pos]) && !is_cmd_seperator(input[*curr_pos]) && input[*curr_pos] != '|' && input[*curr_pos] != '>' && input[*curr_pos] != '<')
		(*curr_pos)++;

	// Return NULL if no token found (prevents empty tokens from being added)
	if (*curr_pos == start)
		return (NULL);

	return (strndup(&input[start], *curr_pos - start));
}

t_cmd	*get_cmd(const char *input, int *curr_pos, t_env *env_list)
{
	t_cmd	*cmd;
	t_list	*new_node;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->tokens = NULL;
	cmd->redirs = NULL;

	while (input[*curr_pos] && !is_cmd_seperator(input[*curr_pos]) && input[*curr_pos] != '|')
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
			char *token = get_token(input, curr_pos);
			if (token)
			{
				// 2) Expand it if it starts with '$'
				char *expanded = expand_env_token(token, env_list); // use your env list
				free(token); // free the old token

				// 3) Create the node with the expanded token
				new_node = ft_lstnew((void *)expanded);
				if (!new_node)
				{
					free(expanded);
					return (NULL);
				}
				ft_lstadd_back(&cmd->tokens, new_node);
			}
		}
	}
	return (cmd);
}

t_command_group	*get_command_group(const char *input, int *curr_pos, t_env *env_list)
{
	t_command_group	*command_group;
	t_list		*cmd;

	command_group = (t_command_group *)malloc(sizeof(t_command_group));
	if (!command_group)
		return (NULL);
	command_group->cmds = NULL;
	command_group->seperator = NULL;

	while (input[*curr_pos] && !is_command_group_seperator(&input[*curr_pos]))
	{
		ft_skip_spaces(input, curr_pos);

		// Parse commands inside this table
		cmd = ft_lstnew((void *)get_cmd(input, curr_pos, env_list));
		if (!cmd)
		{
			free(command_group);
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

	command_group->seperator = get_command_group_seperator(input, curr_pos);
	return (command_group);
}

t_ast	*get_ast(const char *input, t_env *env_list)
{
	t_ast	*ast;
	t_list	*command_group;
	int		curr_pos;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->command_groups = NULL;

	curr_pos = 0;
	while (input[curr_pos])
	{
		ft_skip_spaces(input, &curr_pos);
		command_group = ft_lstnew((void *)get_command_group(input, &curr_pos, env_list));
		if (!command_group)
		{
			free(ast);
			return (NULL);
		}
		ft_lstadd_back(&ast->command_groups, command_group);
	}
	return (ast);
}