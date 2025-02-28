/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/02/28 12:06:01 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void shell_export(char **args, t_env *env_list)
{
    int i = 0;

    // If "export" was called with no args, do nothing or show usage
    if (!args || !args[0])
        return;

    // Process each argument in form "KEY=VALUE"
    while (args[i])
    {
        char *eq = ft_strchr(args[i], '=');

        if (!eq)
        {
            // No '=' means it's just "export KEY" with no value.
            // Decide how you'd like to handle that. For now, skip it.
        }
        else
        {
            // Temporarily split the string at '='
            *eq = '\0';
            // eq+1 is everything after '=', e.g. "VALUE"
            set_env_value(&env_list, args[i], eq + 1);
            // Restore the '=' so args[i] remains unchanged for debugging/logging
            *eq = '=';
        }
        i++;
    }
}
