/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:56:59 by jidler            #+#    #+#             */
/*   Updated: 2025/03/08 22:56:59 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*init_token(void)
{
	t_token	*token_struct;

	token_struct = malloc(sizeof(t_token));
	if (!token_struct)
		return (NULL);
	token_struct->is_single_quoted = 0;
	return (token_struct);
}

char	*allocate_token_buffer(t_token *token_struct)
{
	char	*buffer;

	buffer = ft_calloc(1, sizeof(char));
	if (!buffer)
	{
		free(token_struct);
		return (NULL);
	}
	return (buffer);
}

void	process_quoted_content(const char *input, int *i, char **buffer,
		char quote)
{
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '\\' && (input[*i + 1] == '"'
				|| input[*i + 1] == '\\'))
			(*i)++;
		*buffer = ft_realloc_str(*buffer, ft_strlen(*buffer) + 2);
		ft_strncat(*buffer, &input[*i], 1);
		(*i)++;
	}
}

void	process_token_content(const char *input, int *i, char **buffer,
		t_token *token_struct)
{
	char	quote;

	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			if (quote == '\'')
				token_struct->is_single_quoted = 1;
			(*i)++;
			process_quoted_content(input, i, buffer, quote);
			if (input[*i] == quote)
				(*i)++;
		}
		else
		{
			*buffer = ft_realloc_str(*buffer, ft_strlen(*buffer) + 2);
			ft_strncat(*buffer, &input[*i], 1);
			(*i)++;
		}
	}
}

t_token	*get_token(const char *input, int *curr_pos)
{
	int		start;
	int		i;
	t_token	*token_struct;
	char	*buffer;

	ft_skip_spaces(input, curr_pos);
	start = *curr_pos;
	i = *curr_pos;
	token_struct = init_token();
	if (!token_struct)
		return (NULL);
	buffer = allocate_token_buffer(token_struct);
	if (!buffer)
		return (NULL);
	process_token_content(input, &i, &buffer, token_struct);
	if (i == start)
	{
		free(token_struct);
		free(buffer);
		return (NULL);
	}
	token_struct->value = buffer;
	*curr_pos = i;
	return (token_struct);
}
