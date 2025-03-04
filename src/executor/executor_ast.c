#include "executor.h"

int  exec_ast(t_ast *ast, t_env *env_list)
{
    t_list *command_group_node;
    t_command_group *command_group;
    int exit_stauts=0;

    command_group_node = ast->command_groups;
    while (command_group_node)
    {
        command_group = (t_command_group *)command_group_node->content;
        command_group->pids = NULL;
        /* Pass env_list down to the next function */
        exit_stauts = exec_command_group(command_group, env_list);
        free_command_group(command_group);
        // (Ignoring logical operators for now)
        command_group_node = command_group_node->next;
    }
    return(exit_stauts);
}