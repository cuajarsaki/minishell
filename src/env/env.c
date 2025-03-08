/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:14:25 by jidler            #+#    #+#             */
/*   Updated: 2025/03/08 23:18:21 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	char	*eq;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		*eq = '\0';
		new_node = new_env_node(envp[i], eq + 1);
		*eq = '=';
		if (!new_node)
			return (NULL);
		if (!head)
			head = new_node;
		else
			*((t_env **)((char *)tail + 2 * sizeof(char *))) = new_node;
		tail = new_node;
		i++;
	}
	return (head);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}
