/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:18:46 by pchung            #+#    #+#             */
/*   Updated: 2025/03/08 22:37:56 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void	unset_env_value(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
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
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**convert_env_list_to_array(t_env *env_list)
{
	int		count;
	int		i;
	t_env	*temp;
	char	**envp;
	size_t	len;

	count = env_count(env_list);
	temp = env_list;
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		len = ft_strlen(temp->key) + ft_strlen(temp->value) + 2;
		envp[i] = malloc(len);
		if (!envp[i])
			return (NULL);
		ft_snprintf(envp[i], len, "%s=%s", temp->key, temp->value);
		temp = temp->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}
