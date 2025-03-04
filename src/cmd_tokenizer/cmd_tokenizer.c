/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:07:42 by pchung            #+#    #+#             */
/*   Updated: 2025/03/04 16:13:20 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"


static size_t argv_len(t_cmd *cmd)
{
    size_t len = 0;
    t_list *tmp = cmd->tokens; 
    while (tmp != NULL)
    {
        len++;
        tmp = tmp->next;
    }
    return len;
}

static char	*xstrdup(const char *s1)
{
	char	*tmp;

	tmp = ft_strdup(s1);
	if (tmp == NULL)
		perror("xstrdup");
	return (tmp);
}

char **token_list_to_argv(t_cmd *cmd)
{
    char **argv;
    size_t i;
    size_t len;
    t_list *current;

    len = argv_len(cmd);

    argv = (char **)malloc((len + 1) * sizeof(char *));
    if (argv == NULL)
        perror("token_list_to_argv");

    // Populate the array with the contents of the linked list
    current = cmd->tokens;
    for (i = 0; i < len; i++)
    {
        argv[i] = xstrdup((char *)current->content);
        current = current->next;
    }
    
    // Null-terminate the array
    argv[len] = NULL;

    return argv;
}
