#include "parser.h"

char	*expand_env_token(const char *token, t_env *env_list, int *exit_status)
{
	char	*result;
	char	*var_value;
	char	*var_name;
	int		i = 0, j;
	int		len = ft_strlen(token);

	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);

	while (i < len)
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			char fixed_value[12];
			snprintf(fixed_value, sizeof(fixed_value), "%d", *exit_status);
			result = ft_realloc_str(result, ft_strlen(result) + ft_strlen(fixed_value) + 1);
			strcat(result, fixed_value);
			i += 2;
		}
		else if (token[i] == '$' && token[i + 1] && (isalnum(token[i + 1]) || token[i + 1] == '_'))
		{
			j = i + 1;
			while (token[j] && (isalnum(token[j]) || token[j] == '_'))
				j++;
			var_name = strndup(&token[i + 1], j - i - 1);
			var_value = get_env_value(env_list, var_name);
			free(var_name);

			if (var_value)
				result = ft_realloc_str(result, ft_strlen(result) + ft_strlen(var_value) + 1);
			else
				result = ft_realloc_str(result, ft_strlen(result) + 1);
			strcat(result, var_value ? var_value : "");

			i = j;
		}
		else
		{
			size_t new_len = ft_strlen(result) + 2;
			result = ft_realloc_str(result, new_len);
			strncat(result, &token[i], 1);
			i++;
		}
	}

	return result;
}
