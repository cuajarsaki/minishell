/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:01:00 by pchung            #+#    #+#             */
/*   Updated: 2025/03/04 16:15:13 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../shell.h"

void free_argv(char **argv)
{
    size_t i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

static void do_nothing(void *__ptr)
{
    (void)__ptr;
    return;
}

void free_redir(void *ptr)
{
    t_redir *redir = (t_redir *)ptr;
    if (redir)
    {
        if (redir->direction)
            free(redir->direction);
        free(redir);
    }
}

void free_cmd(t_cmd *cmd)
{
    if (cmd)
    {
        ft_lstclear(&cmd->tokens, free);
        ft_lstclear(&cmd->redirs, free_redir); 
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
    }
}