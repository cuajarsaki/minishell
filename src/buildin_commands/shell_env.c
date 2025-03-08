/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:54:09 by pchung            #+#    #+#             */
/*   Updated: 2025/03/06 11:54:43 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	shell_env(t_env *env_list)
{
	if (!env_list)
	{
		printf("No environment variables to display.\n");
		return (1);
	}
	while (env_list)
	{
		printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}
