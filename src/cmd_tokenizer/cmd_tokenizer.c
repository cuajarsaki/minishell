/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:07:42 by pchung            #+#    #+#             */
/*   Updated: 2025/03/06 21:56:49 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static size_t	argv_len(t_cmd *cmd)
{
	size_t	len;
	t_list	*tmp;

	len = 0;
	tmp = cmd->tokens;
	while (tmp != NULL)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

static char	*xstrdup(const char *s1)
{
	char	*tmp;

	tmp = ft_strdup(s1);
	if (tmp == NULL)
		perror("xstrdup");
	return (tmp);
}

char	**token_list_to_argv(t_cmd *cmd)
{
	char	**argv;
	size_t	i;
	size_t	len;
	t_list	*current;

	len = argv_len(cmd);
	argv = (char **)malloc((len + 1) * sizeof(char *));
	if (argv == NULL)
		perror("token_list_to_argv");
	current = cmd->tokens;
	i = 0;
	while (i < len)
	{
		argv[i] = xstrdup((char *)current->content);
		current = current->next;
		i++;
	}
	argv[len] = NULL;
	return (argv);
}
