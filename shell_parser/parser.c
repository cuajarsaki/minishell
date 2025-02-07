/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:44:55 by jidler            #+#    #+#             */
/*   Updated: 2025/02/07 14:09:32 by jidler           ###   ########.fr       */
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

int		is_cmd_delimiter(char c)
{
	return (c == ';' || c == '&');
}

int		is_cmd_table_delimiter(const char *str)
{
	return (strncmp(str, "||", 2) == 0 || strncmp(str, "&&", 2) == 0 || *str == ';');
}

char	*get_cmd_table_delimiter(const char *input, int *curr_pos)
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
	while (input[*curr_pos] && !isspace(input[*curr_pos]) && !is_cmd_delimiter(input[*curr_pos]) && input[*curr_pos] != '|')
		(*curr_pos)++;

	redir->direction = strndup(&input[start], *curr_pos - start);
	return (redir);
}

char	*get_token(const char *input, int *curr_pos)
{
	ft_skip_spaces(input, curr_pos); // Skip leading spaces before reading a token
	int start = *curr_pos;

	// Extract token until space, command delimiter, or redirection
	while (input[*curr_pos] && !isspace(input[*curr_pos]) && !is_cmd_delimiter(input[*curr_pos]) && input[*curr_pos] != '|' && input[*curr_pos] != '>' && input[*curr_pos] != '<')
		(*curr_pos)++;

	// Return NULL if no token found (prevents empty tokens from being added)
	if (*curr_pos == start)
		return (NULL);

	return (strndup(&input[start], *curr_pos - start));
}

t_cmd	*get_cmd(const char *input, int *curr_pos)
{
	t_cmd	*cmd;
	t_list	*new_node;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->tokens = NULL;
	cmd->redirs = NULL;

	while (input[*curr_pos] && !is_cmd_delimiter(input[*curr_pos]) && input[*curr_pos] != '|')
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
				new_node = ft_lstnew((void *)token);
				if (!new_node)
					return (NULL);
				ft_lstadd_back(&cmd->tokens, new_node);
			}
		}
	}
	return (cmd);
}

t_cmd_table	*get_cmd_table(const char *input, int *curr_pos)
{
	t_cmd_table	*cmd_table;
	t_list		*cmd;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		return (NULL);
	cmd_table->cmds = NULL;
	cmd_table->delimiter = NULL;

	while (input[*curr_pos] && !is_cmd_table_delimiter(&input[*curr_pos]))
	{
		ft_skip_spaces(input, curr_pos);

		// Parse commands inside this table
		cmd = ft_lstnew((void *)get_cmd(input, curr_pos));
		if (!cmd)
		{
			free(cmd_table);
			return (NULL);
		}
		ft_lstadd_back(&cmd_table->cmds, cmd);

		// Handle pipes within the same command table
		if (input[*curr_pos] == '|' && input[*curr_pos + 1] != '|')
		{
			(*curr_pos)++; // Skip the `|`
			continue; // Stay inside the same `t_cmd_table`
		}
	}

	cmd_table->delimiter = get_cmd_table_delimiter(input, curr_pos);
	return (cmd_table);
}

t_ast	*get_ast(const char *input)
{
	t_ast	*ast;
	t_list	*cmd_table;
	int		curr_pos;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->cmd_tables = NULL;

	curr_pos = 0;
	while (input[curr_pos])
	{
		ft_skip_spaces(input, &curr_pos);
		cmd_table = ft_lstnew((void *)get_cmd_table(input, &curr_pos));
		if (!cmd_table)
		{
			free(ast);
			return (NULL);
		}
		ft_lstadd_back(&ast->cmd_tables, cmd_table);
	}
	return (ast);
}