/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:18:46 by pchung            #+#    #+#             */
/*   Updated: 2025/03/06 10:18:54 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../shell.h"

// Function to free environment list
void free_env_list(t_env *env_list)
{
    while (env_list)
    {
        t_env *temp = env_list;
        env_list = env_list->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

void set_env_value(t_env **env_list, const char *key, const char *value)
{
    t_env *current = *env_list;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            if (value)
                current->value = ft_strdup(value);
            else
                current->value = ft_strdup("");
            return;
        }
        current = current->next;
    }

    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = ft_strdup("");
    new_node->next = NULL;

    if (!*env_list)
        *env_list = new_node;
    else
    {
        current = *env_list;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

void unset_env_value(t_env **env_list, const char *key)
{
    t_env *current = *env_list, *prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;

            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

char **convert_env_list_to_array(t_env *env_list)
{
    int count = 0;
    t_env *temp = env_list;
    while (temp) {
        count++;
        temp = temp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    if (!envp)
        return NULL;

    temp = env_list;
    for (int i = 0; i < count; i++)
    {
        size_t len = ft_strlen(temp->key) + ft_strlen(temp->value) + 2;
        envp[i] = malloc(len);
        if (!envp[i])
            return NULL;
        ft_snprintf(envp[i], len, "%s=%s", temp->key, temp->value);
        temp = temp->next;
    }
    envp[count] = NULL;
    return envp;
}

void free_env_array(char **envp)
{
    for (int i = 0; envp[i]; i++)
        free(envp[i]);
    free(envp);
}

