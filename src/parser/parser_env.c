/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:09:32 by jidler            #+#    #+#             */
/*   Updated: 2025/03/08 23:01:03 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*handle_exit_status_expansion(char *result, int *exit_status)
{
	char	fixed_value[12];

	ft_snprintf(fixed_value, sizeof(fixed_value), "%d", *exit_status);
	result = ft_realloc_str(result, ft_strlen(result) + ft_strlen(fixed_value)
			+ 1);
	strcat(result, fixed_value);
	return (result);
}

char	*extract_variable_name(const char *token, int *i)
{
	char	*var_name;
	int		j;

	j = *i + 1;
	while (token[j] && (isalnum(token[j]) || token[j] == '_'))
		j++;
	var_name = strndup(&token[*i + 1], j - *i - 1);
	*i = j;
	return (var_name);
}

char	*expand_variable(char *result, const char *token, int *i,
		t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	char	*temp_result;
	size_t	new_size;

	var_name = extract_variable_name(token, i);
	var_value = get_env_value(env_list, var_name);
	free(var_name);
	new_size = ft_strlen(result) + 1;
	if (var_value)
		new_size += ft_strlen(var_value);
	temp_result = ft_realloc_str(result, new_size);
	if (var_value)
		strcat(temp_result, var_value);
	return (temp_result);
}

char	*process_regular_characters(char *result, const char *token, int *i)
{
	size_t	new_len;

	new_len = ft_strlen(result) + 2;
	result = ft_realloc_str(result, new_len);
	strncat(result, &token[*i], 1);
	(*i)++;
	return (result);
}

char	*expand_env_token(const char *token, t_env *env_list, int *exit_status)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			result = handle_exit_status_expansion(result, exit_status);
			i += 2;
		}
		else if (token[i] == '$' && token[i + 1] && (isalnum(token[i + 1])
				|| token[i + 1] == '_'))
		{
			result = expand_variable(result, token, &i, env_list);
		}
		else
		{
			result = process_regular_characters(result, token, &i);
		}
	}
	return (result);
}
