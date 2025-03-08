/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:53:50 by pchung            #+#    #+#             */
/*   Updated: 2025/03/08 22:26:53 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static void	update_env_node(t_env *node, const char *value)
{
	free(*((char **)((char *)node + sizeof(char *))));
	if (value)
		*((char **)((char *)node + sizeof(char *))) = ft_strdup(value);
	else
		*((char **)((char *)node + sizeof(char *))) = ft_strdup("");
}

static void	append_node(t_env **env_list, t_env *new_node)
{
	t_env	*cur;

	cur = *env_list;
	if (!cur)
	{
		*env_list = new_node;
		return ;
	}
	while (*((t_env **)((char *)cur + 2 * sizeof(char *))))
		cur = *((t_env **)((char *)cur + 2 * sizeof(char *)));
	*((t_env **)((char *)cur + 2 * sizeof(char *))) = new_node;
}

void	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*new_node;

	cur = *env_list;
	while (cur)
	{
		if (ft_strcmp(*((char **)((char *)cur)), key) == 0)
		{
			update_env_node(cur, value);
			return ;
		}
		cur = *((t_env **)((char *)cur + 2 * sizeof(char *)));
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	*((char **)((char *)new_node)) = ft_strdup(key);
	if (value)
		*((char **)((char *)new_node + sizeof(char *))) = ft_strdup(value);
	else
		*((char **)((char *)new_node + sizeof(char *))) = ft_strdup("");
	*((t_env **)((char *)new_node + 2 * sizeof(char *))) = 0;
	append_node(env_list, new_node);
}
