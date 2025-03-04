#include "parser.h"

t_redir	*get_redir(const char *input, int *curr_pos)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);

	if (ft_strncmp(&input[*curr_pos], ">>", 2) == 0)
	{
		strcpy(redir->type, ">>");
		*curr_pos += 2;
	}
	else if (ft_strncmp(&input[*curr_pos], "<<", 2) == 0)
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

	int start = *curr_pos;
	while (input[*curr_pos] && !isspace(input[*curr_pos]) && input[*curr_pos] != '|')
		(*curr_pos)++;

	redir->direction = ft_strndup(&input[start], *curr_pos - start);
	return (redir);
}
