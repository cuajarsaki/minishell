#include "parser.h"

t_token	*get_token(const char *input, int *curr_pos)
{
	ft_skip_spaces(input, curr_pos);

	int		start = *curr_pos;
	int		i = *curr_pos;
	char	quote = 0;
	t_token	*token_struct = malloc(sizeof(t_token));

	if (!token_struct)
		return (NULL);
	token_struct->is_single_quoted = 0;

	char	*buffer = ft_calloc(1, sizeof(char));
	if (!buffer)
	{
		free(token_struct);
		return (NULL);
	}

	while (input[i] && !isspace(input[i]) && input[i] != '|' && input[i] != '>' && input[i] != '<')
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			if (quote == '\'')
				token_struct->is_single_quoted = 1;
			i++;

			while (input[i] && input[i] != quote)
			{
				if (quote == '"' && input[i] == '\\' && (input[i + 1] == '"' || input[i + 1] == '\\'))
					i++;

				size_t new_len = ft_strlen(buffer) + 2;
				buffer = ft_realloc_str(buffer, new_len);
				ft_strncat(buffer, &input[i], 1);
				i++;
			}

			if (input[i] == quote)
				i++;
		}
		else
		{
			size_t new_len = ft_strlen(buffer) + 2;
			buffer = ft_realloc_str(buffer, new_len);
			ft_strncat(buffer, &input[i], 1);
			i++;
		}
	}

	if (i == start)
	{
		free(token_struct);
		free(buffer);
		return (NULL);
	}

	token_struct->value = buffer;
	*curr_pos = i;

	return token_struct;
}
