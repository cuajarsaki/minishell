/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:14:25 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 17:31:17 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../shell.h"


// Function to create a new node
t_env *new_env_node(char *key, char *value)
{
    t_env *new = (t_env *)malloc(sizeof(t_env));
    if (!new)
        return NULL;
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = NULL;
    return new;
}

t_env *init_env_list(char **envp) // Use envp instead of environ
{
    t_env *head = NULL, *tail = NULL;

    for (int i = 0; envp[i]; i++) // Iterate over envp
    {
        char *eq = ft_strchr(envp[i], '=');
        if (!eq)
            continue;

        *eq = '\0'; // Temporarily break into key and value
        t_env *new_node = new_env_node(envp[i], eq + 1);
        *eq = '=';  // Restore original format

        if (!new_node)
            return NULL;

        if (!head)
            head = new_node;
        else
            tail->next = new_node;

        tail = new_node;
    }
    return head;
}


// Function to initialize environment list from environ
// t_env *init_env_list(char ** environ)
// {
//     t_env *head = NULL, *tail = NULL;
//     for (int i = 0; environ[i]; i++)
//     {
//         char *eq = ft_strchr(environ[i], '=');
//         if (!eq)
//             continue;
//         *eq = '\0';
//         t_env *new_node = new_env_node(environ[i], eq + 1);
//         *eq = '='; // Restore original string
//         if (!new_node)
//             return NULL;
//         if (!head)
//             head = new_node;
//         else
//             tail->next = new_node;
//         tail = new_node;
//     }
//     return head;
// }

// Function to get value of an env variable
char *get_env_value(t_env *env_list, const char *key)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->key, key) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}
