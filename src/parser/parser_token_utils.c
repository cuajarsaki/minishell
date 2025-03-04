#include "parser.h"

char	*remove_quotes(const char *token)
{
	int		len;
	char	*new_token;

	if (!token)
		return (NULL);

	len = ft_strlen(token);
	if (len < 2)
		return ft_strdup(token);

	if ((token[0] == '\'' || token[0] == '\"') && token[0] == token[len - 1])
	{

		new_token = ft_strndup(token + 1, len - 2);
	}
	else
		new_token = ft_strdup(token);

	return new_token;
}
