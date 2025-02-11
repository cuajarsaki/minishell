/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:01:00 by pchung            #+#    #+#             */
/*   Updated: 2025/02/12 00:16:40 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void do_nothing(void *__ptr)
{
    (void)__ptr;
    return;
}

void free_cmd(t_cmd *cmd)
{
    if (cmd)
    {
        ft_lstclear(&cmd->tokens, free);
        ft_lstclear(&cmd->redirs, free);
        free(cmd);
    }
}

void free_ast(t_ast *ast)
{
    if (ast)
    {
        ft_lstclear(&ast->command_groups, free);
        free(ast);
    }
}

void free_command_group(t_command_group *command_group)
{
    if (command_group)
    {
        ft_lstclear(&command_group->cmds, (void (*)(void *))free_cmd);
        ft_lstclear(&command_group->pids, do_nothing);
        free(command_group->seperator);
    }
}