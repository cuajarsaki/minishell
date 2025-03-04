/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:01:57 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 17:03:59 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void free_ast(t_ast *ast)
{
    if (ast)
    {
        ft_lstclear(&ast->command_groups, free);
        free(ast);
    }
}

static void do_nothing(void *__ptr)
{
    (void)__ptr;
    return;
}

void free_command_group(t_command_group *command_group)
{
    if (command_group)
    {
        ft_lstclear(&command_group->cmds, (void (*)(void *))free_cmd);
        ft_lstclear(&command_group->pids, do_nothing);
    }
}

