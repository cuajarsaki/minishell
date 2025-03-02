/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:07:42 by pchung            #+#    #+#             */
/*   Updated: 2025/03/02 19:43:44 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"


// static void skip_empty_word_is_expanted_true(t_cmd **tok)
// {
//     t_cmd *current = *tok;
//     t_list *prev = NULL;
//     t_list *node = current->tokens;

//     while (node)
//     {
//         if (node->content == NULL || strlen((char *)node->content) == 0)
//         {
//             // Skip the empty word
//             if (prev)
//                 prev->next = node->next;
//             else
//                 current->tokens = node->next;
//             t_list *temp = node;
//             node = node->next;
//             free(temp);
//         }
//         else
//         {
//             prev = node;
//             node = node->next;
//         }
//     }
// }

static bool at_eof(t_cmd *cmd)
{
    return cmd->tokens == NULL;
}

static size_t argv_len(t_cmd *cmd)
{
    size_t len = 0;
    while (cmd && !at_eof(cmd))
    {
        len++;
        cmd->tokens = cmd->tokens->next;
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

    // Calculate the length of the array
    len = argv_len(cmd);

    // Allocate memory for the array
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
