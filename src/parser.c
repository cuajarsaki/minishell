/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:44:55 by jidler            #+#    #+#             */
/*   Updated: 2025/03/01 03:35:44 by pchung           ###   ########.fr       */
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
	return (ft_strncmp(str, "||", 2) == 0 || ft_strncmp(str, "&&", 2) == 0 || *str == ';');
}

char	*get_command_group_seperator(const char *input, int *curr_pos)
{
	if (input[*curr_pos] == ';')
	{
		(*curr_pos)++;
		return (ft_strdup(";"));
	}
	else if (ft_strncmp(&input[*curr_pos], "||", 2) == 0)
	{
		*curr_pos += 2;
		return (ft_strdup("||"));
	}
	else if (ft_strncmp(&input[*curr_pos], "&&", 2) == 0)
	{
		*curr_pos += 2;
		return (ft_strdup("&&"));
	}
	return (ft_strdup(""));
}


char *ft_realloc_str(char *ptr, size_t new_size)
{
    char *new_ptr;

    // Allocate new memory
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

    // If ptr was NULL, just return the new allocation
    if (!ptr)
    {
        new_ptr[0] = '\0'; // Ensure empty string
        return new_ptr;
    }

    // Copy old content safely
    ft_strlcpy(new_ptr, ptr, new_size);

    // Free old memory
    free(ptr);

    return new_ptr;
}


char	*expand_env_token(const char *token, t_env *env_list)
{
	char	*result;
	char	*var_value;
	char	*var_name;
	int		i = 0, j;
	int		len = ft_strlen(token);

	// Allocate result buffer (initially empty)
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);

	while (i < len)
	{
		// If we find a '$' and it's not inside single quotes
		if (token[i] == '$' && token[i + 1] && (isalnum(token[i + 1]) || token[i + 1] == '_'))
		{
			j = i + 1;
			// Find the end of the variable name (alphanumeric + '_')
			while (token[j] && (isalnum(token[j]) || token[j] == '_'))
				j++;

			// Extract variable name
			var_name = strndup(&token[i + 1], j - i - 1);
			var_value = get_env_value(env_list, var_name);
			free(var_name);

			// If variable exists, append its value; else, append an empty string
			if (var_value)
				result = ft_realloc_str(result, ft_strlen(result) + ft_strlen(var_value) + 1);
			else
				result = ft_realloc_str(result, ft_strlen(result) + 1);

			strcat(result, var_value ? var_value : "");

			i = j; // Move past variable
		}
		else
		{
			// Append non-variable characters
			size_t new_len = ft_strlen(result) + 2;
			result = ft_realloc_str(result, new_len);
			strncat(result, &token[i], 1);
			i++;
		}
	}

	return result;
}


// static char *expand_env_token(const char *token, t_env *env_list)
// {
//     // 1) If token is NULL or empty, just return a copy
//     if (!token || !*token)
//         return (token ? ft_strdup(token) : NULL);

//     // 2) If token does NOT start with '$', return a copy unchanged
//     if (token[0] != '$')
//         return ft_strdup(token);

//     // 3) Extract the variable name (everything after '$')
//     const char *var_name = token + 1;

//     // Edge case: if the token is just "$", no var_name
//     if (!*var_name)
//         return ft_strdup("$"); // or return ft_strdup("") depending on your shell’s behavior

//     // 4) Look up var_name in env_list
//     char *val = get_env_value(env_list, var_name);

//     // If not found, some shells use an empty string
//     if (!val)
//         val = "";

//     // 5) Return a newly allocated copy of the expansion
//     return ft_strdup(val);
// }

char	*remove_quotes(const char *token)
{
	int		len;
	char	quote;
	char	*new_token;

	(void)quote;
	if (!token)
		return (NULL);

	len = ft_strlen(token);
	if (len < 2)
		return ft_strdup(token); // No quotes to remove

	// Check if token starts and ends with the same quote
	if ((token[0] == '\'' || token[0] == '\"') && token[0] == token[len - 1])
	{
		quote = token[0];
		// ✅ Remove only the first and last quotes, keep everything else
		new_token = strndup(token + 1, len - 2);
	}
	else
	{
		// No matching surrounding quotes, return as is
		new_token = ft_strdup(token);
	}

	return new_token;
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
	if (ft_strncmp(&input[*curr_pos], ">>", 2) == 0)
	{
		strcpy(redir->type, ">>");
		*curr_pos += 2;
	}
	if (ft_strncmp(&input[*curr_pos], "<<", 2) == 0)
	{
		strcpy(redir->type, "<<");
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


// char	*get_token(const char *input, int *curr_pos)
// {
// 	ft_skip_spaces(input, curr_pos); // Skip leading spaces

// 	int		start = *curr_pos;
// 	int		i = *curr_pos;
// 	char	quote = 0; // Variable to track open quote

// 	// Check if the first character is a quote
// 	if (input[i] == '\'' || input[i] == '\"')
// 	{
// 		quote = input[i]; // Store the type of quote used
// 		i++; // Move past opening quote

// 		// Find the closing quote
// 		while (input[i] && input[i] != quote)
// 			i++;

// 		// If found, move past the closing quote
// 		if (input[i] == quote)
// 			i++;
// 	}
// 	else
// 	{
// 		// Read normally until a space, redirection, or separator
// 		while (input[i] && !isspace(input[i]) && !is_cmd_seperator(input[i]) && input[i] != '|' && input[i] != '>' && input[i] != '<')
// 			i++;
// 	}

// 	// No token found
// 	if (i == start)
// 		return (NULL);

// 	// Extract the raw token
// 	char *raw_token = strndup(&input[start], i - start);
// 	*curr_pos = i; // Move position forward

// 	// ✅ Remove quotes from the token before returning
// 	char *clean_token = remove_quotes(raw_token);
// 	free(raw_token);
// 	return clean_token;
// }

typedef struct s_token
{
	char	*value;
	int		is_single_quoted;
}	t_token;


t_token	*get_token(const char *input, int *curr_pos)
{
	ft_skip_spaces(input, curr_pos); // Skip leading spaces

	int		start = *curr_pos;
	int		i = *curr_pos;
	char	quote = 0;
	t_token	*token_struct = malloc(sizeof(t_token));

	if (!token_struct)
		return (NULL);
	token_struct->is_single_quoted = 0;

	// Allocate buffer to store final token
	char	*buffer = ft_calloc(1, sizeof(char));
	if (!buffer)
	{
		free(token_struct);
		return (NULL);
	}

	// Read token, handling quotes and escape sequences
	while (input[i] && !isspace(input[i]) && !is_cmd_seperator(input[i]) && input[i] != '|' && input[i] != '>' && input[i] != '<')
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			if (quote == '\'')
				token_struct->is_single_quoted = 1;

			i++; // Move past opening quote

			// Process quoted content
			while (input[i] && input[i] != quote)
			{
				// ✅ **Correctly handle `\"` inside double quotes (remove `\` before `"` and `\`)**
				if (quote == '"' && input[i] == '\\' && (input[i + 1] == '"' || input[i + 1] == '\\'))
					i++; // Skip the backslash

				// Append the character (escaped or not) to the buffer
				size_t new_len = ft_strlen(buffer) + 2;
				buffer = ft_realloc_str(buffer, new_len);
				strncat(buffer, &input[i], 1);
				i++;
			}

			// Move past closing quote
			if (input[i] == quote)
				i++;
		}
		else if (input[i] == '\\' && (input[i + 1] == '"' || input[i + 1] == '\\')) // ✅ **Remove `\` before `"` or `\`**
		{
			i++; // Skip the backslash
			// Append only the escaped character
			size_t new_len = ft_strlen(buffer) + 2;
			buffer = ft_realloc_str(buffer, new_len);
			strncat(buffer, &input[i], 1);
			i++;
		}
		else
		{
			// Append normal character to buffer
			size_t new_len = ft_strlen(buffer) + 2;
			buffer = ft_realloc_str(buffer, new_len);
			strncat(buffer, &input[i], 1);
			i++;
		}
	}

	// No token found
	if (i == start)
	{
		free(token_struct);
		free(buffer);
		return (NULL);
	}

	// Store the final token value
	token_struct->value = buffer;
	*curr_pos = i; // Move position forward

	return token_struct;
}

// char	*get_token(const char *input, int *curr_pos)
// {
// 	ft_skip_spaces(input, curr_pos); // Skip leading spaces before reading a token
// 	int start = *curr_pos;

// 	// Extract token until space, command seperator, or redirection
// 	while (input[*curr_pos] && !isspace(input[*curr_pos]) && !is_cmd_seperator(input[*curr_pos]) && input[*curr_pos] != '|' && input[*curr_pos] != '>' && input[*curr_pos] != '<')
// 		(*curr_pos)++;

// 	// Return NULL if no token found (prevents empty tokens from being added)
// 	if (*curr_pos == start)
// 		return (NULL);

// 	return (strndup(&input[start], *curr_pos - start));
// }

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
			if (!new_node){
				return (NULL);
			}
			ft_lstadd_back(&cmd->redirs, new_node);
		}
		else
		{
			t_token *token_struct = get_token(input, curr_pos);
			if (token_struct)
			{
				printf("[Token: %s]\n", token_struct->value);

				// ✅ Expand environment variables **ONLY if not single-quoted**
				char *final_token;
				if (token_struct->is_single_quoted)
				{
					final_token = ft_strdup(token_struct->value); // Preserve as-is
				}
				else
				{
					final_token = expand_env_token(token_struct->value, env_list);
				}

				free(token_struct->value);
				free(token_struct);

				// ✅ Store the processed token
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