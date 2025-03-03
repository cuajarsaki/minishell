/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 01:03:28 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

int shell_unset(char **args, t_env *env_list)
{
    int i = 0;

    // If "unset" was called without any arguments, do nothing
    if (!args || !args[0])
        return 1;

    // For each argument (which should be the name of a variable to remove)
    while (args[i])
    {
        // Note that 'unset_env_value' takes t_env**, so pass &env_list
        unset_env_value(&env_list, args[i]);
        i++;
    }
    return 0;
}
