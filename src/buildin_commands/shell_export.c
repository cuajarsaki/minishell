/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/06 10:24:16 by pchung           ###   ########.fr       */
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
        if (ft_strlen(args[i]) == 0 || args[i][0] == '=')
        {
            printf("export: `%s': not a valid identifier\n", args[i]);
            return 1;
        }
        else
        {
            char *eq = ft_strchr(args[i], '=');
            if (eq)
            {
                *eq = '\0';
                set_env_value(&env_list, args[i], eq + 1);
                *eq = '=';
            }
            else
            {
                set_env_value(&env_list, args[i], NULL);
            }
        }
        i++;
    }
    return 0;
}
