/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:14:25 by jidler            #+#    #+#             */
/*   Updated: 2025/03/02 15:28:17 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../shell.h"

extern char **environ;

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

// Function to initialize environment list from environ
t_env *init_env_list(void)
{
    t_env *head = NULL, *tail = NULL;
    for (int i = 0; environ[i]; i++)
    {
        char *eq = ft_strchr(environ[i], '=');
        if (!eq)
            continue;
        *eq = '\0';
        t_env *new_node = new_env_node(environ[i], eq + 1);
        *eq = '='; // Restore original string
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
        if (ft_strcmp(current->key, key) == 0) // If variable exists, update it
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }

    // If key does not exist, create new node
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    // Append new node to the list
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
        snprintf(envp[i], len, "%s=%s", temp->key, temp->value);
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
