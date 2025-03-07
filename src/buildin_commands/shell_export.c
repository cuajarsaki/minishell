/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:41:50 by jidler            #+#    #+#             */
/*   Updated: 2025/03/06 13:01:48 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

static void	print_env_list(t_env *env_list)
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

static int	export_arg(char *arg, t_env **env_list)
{
	char	*eq;

	if (ft_strlen(arg) == 0 || arg[0] == '=')
	{
		printf("export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		set_env_value(env_list, arg, eq + 1);
		*eq = '=';
	}
	else
		set_env_value(env_list, arg, NULL);
	return (0);
}

int	shell_export(char **args, t_env *env_list)
{
	int	i;

	i = 1;
	if (!args || !args[1])
	{
		print_env_list(env_list);
		return (0);
	}
	while (args[i])
	{
		if (export_arg(args[i], &env_list))
			return (1);
		i++;
	}
	return (0);
}
