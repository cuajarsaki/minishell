#include "../shell.h"

/* ************************************************************************** */
/*                          FUNCTION DECLARATIONS                             */
/* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list);
void exec_command_group(t_command_group *command_group, t_env *env_list);
void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
void exec_cmd_builtin(t_cmd *cmd, t_env *env_list);
void exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list);
int  is_builtin(t_cmd *cmd);
void exec_parent(t_list **pids);
char **convert_list_to_arr(t_list *lst);

/* ************************************************************************** */
/*                  🏆 EXECUTE THE ENTIRE AST (MAIN EXECUTION)                */
/* ************************************************************************** */

void exec_ast(t_ast *ast, t_env *env_list)
{
    t_list *command_group_node;
    t_command_group *command_group;
    char *seperator;

    command_group_node = ast->command_groups;
    while (command_group_node)
    {
        command_group = (t_command_group *)command_group_node->content;
        /* Pass env_list down to the next function */
        exec_command_group(command_group, env_list);

        seperator = command_group->seperator;
        // (Ignoring logical operators for now)
        command_group_node = command_group_node->next;
    }
}

/* ************************************************************************** */
/*                 🏆 EXECUTE A SINGLE COMMAND TABLE                          */
/* ************************************************************************** */

void exec_command_group(t_command_group *command_group, t_env *env_list)
{
    t_list *cmds;
    int i;

    cmds = command_group->cmds;
    command_group->cmd_amount = ft_lstsize(cmds);
    i = 0;

    while (cmds)
    {
        /* Pass env_list into exec_cmd */
        exec_cmd((t_cmd *)cmds->content, command_group, i, env_list);
        cmds = cmds->next;
        i++;
    }

    exec_parent(&command_group->pids); // Wait for processes
}

/* ************************************************************************** */
/*                  🏆 EXECUTE A SINGLE COMMAND                               */
/* ************************************************************************** */

void exec_cmd(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
{
    if (cmd->tokens != NULL)
    {
        /* Check if built-in */
        if (is_builtin(cmd)){
			printf("Exectuing BuildIN\n");
            exec_cmd_builtin(cmd, env_list);

		}
        else
		{
			printf("Exectuing External\n");
            exec_cmd_external(cmd, command_group, process_index, env_list);
		}
    }
}

/* ************************************************************************** */
/*              🏆 EXECUTE BUILT-IN COMMANDS (NO FORKING REQUIRED)            */
/* ************************************************************************** */

void exec_cmd_builtin(t_cmd *cmd, t_env *env_list)
{
    char *program = (char *)cmd->tokens->content;

    if (strcmp(program, "echo") == 0)
        shell_echo(convert_list_to_arr(cmd->tokens->next));
    else if (strcmp(program, "cd") == 0)
        shell_cd((char *)cmd->tokens->next->content);
    else if (strcmp(program, "pwd") == 0)
        shell_pwd();
    else if (strcmp(program, "export") == 0)
        shell_export(convert_list_to_arr(cmd->tokens->next), env_list);
    else if (strcmp(program, "unset") == 0)
        shell_unset(convert_list_to_arr(cmd->tokens->next), env_list);
    else if (strcmp(program, "env") == 0)
        shell_env(env_list);
}

/* ************************************************************************** */
/*             🏆 EXECUTE EXTERNAL PROGRAMS (REQUIRES FORKING)                */
/* ************************************************************************** */

void exec_cmd_external(t_cmd *cmd, t_command_group *command_group, int process_index, t_env *env_list)
{
    /* Convert tokens to a NULL-terminated array for execvp */
    char **tokens = convert_list_to_arr(cmd->tokens);
    pid_t pid = fork();

    /* If you do not need env_list here, you can safely ignore it or remove. */
    (void)env_list;

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Save PID in the command_group's pids list to wait on later */
        ft_lstadd_back(&command_group->pids, ft_lstnew((void *)(intptr_t)pid));
        free(tokens);
    }
}

/* ************************************************************************** */
/*              🏆 WAIT FOR ALL CHILD PROCESSES TO FINISH                     */
/* ************************************************************************** */

void exec_parent(t_list **pids)
{
    int status;
    pid_t pid;

    while (*pids)
    {
        pid = (pid_t)(intptr_t)(*pids)->content;
        waitpid(pid, &status, 0);
        *pids = (*pids)->next;
    }
}

/* ************************************************************************** */
/*              🏆 HELPER FUNCTIONS FOR COMMAND EXECUTION                     */
/* ************************************************************************** */

/* Check if command is a built-in */
int is_builtin(t_cmd *cmd)
{
    char *program = (char *)cmd->tokens->content;

    /*
        NOTE: Currently, this returns '0' for these commands,
        meaning "not recognized as builtin." If you want them
        recognized as built-ins, return 1 for matches instead.
    */
    if (!strcmp(program, "echo")   ||
        !strcmp(program, "cd")     ||
        !strcmp(program, "pwd")    ||
        !strcmp(program, "export") ||
        !strcmp(program, "unset")  ||
        !strcmp(program, "env")    ||
        !strcmp(program, "exit"))
    {
        return 1; // <--- This effectively says "Not builtin" in your logic
    }
    return 0;     // <--- Everything else also returns "Not builtin"
}

/* Convert linked list to a NULL-terminated C-string array */
char **convert_list_to_arr(t_list *lst)
{
    int len = ft_lstsize(lst);
    char **arr = (char **)malloc(sizeof(char *) * (len + 1));
    int i = 0;

    while (lst)
    {
        arr[i++] = (char *)lst->content;
        lst = lst->next;
    }
    arr[i] = NULL;
    return arr;
}
