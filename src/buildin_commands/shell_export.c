/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 02:17:38 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static void print_env_list(t_env *env_list)
{
    while (env_list)
    {
        printf("declare -x %s", env_list->key);
        if (env_list->value)
            printf("=\"%s\"", env_list->value);
        printf("\n");
        env_list = env_list->next;
    }
}

int shell_export(char **args, t_env *env_list)
{
    int i = 1;

    if (!args || !args[1])
    {
        print_env_list(env_list);
        return 0;
    }

    while (args[i])
    {
        char *eq = ft_strchr(args[i], '=');
        if (!eq)
        {
            set_env_value(&env_list, args[i], NULL);
        }
        else
        {
            *eq = '\0';
            set_env_value(&env_list, args[i], eq + 1);
            *eq = '=';
        }
        i++;
    }
    return 0;
}
