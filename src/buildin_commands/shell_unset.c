/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/08 21:11:24 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int	shell_unset(char **args, t_env *env_list)
{
	int	i;

	i = 1;
	if (!args || !args[0])
		return (1);
	while (args[i])
	{
		unset_env_value(&env_list, args[i]);
		i++;
	}
	return (0);
}
