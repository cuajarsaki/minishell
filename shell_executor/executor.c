#include "../shell.h"

/* Function Declarations */
void exec_ast(t_ast *ast);
void exec_cmd_table(t_cmd_table *cmd_table);
void exec_cmd(t_cmd *cmd, t_cmd_table *cmd_table, int process_index);
void exec_builtin(t_cmd *cmd);
void exec_external(t_cmd *cmd, t_cmd_table *cmd_table, int process_index);
int is_builtin(t_cmd *cmd);
void exec_parent(t_list **pids);
char **convert_list_to_arr(t_list *lst);

/* ************************************************************************** */
/*                  🏆 EXECUTE THE ENTIRE AST (MAIN EXECUTION)                */
/* ************************************************************************** */
void exec_ast(t_ast *ast)
{
    t_list *cmd_table_node;
    t_cmd_table *cmd_table;
    char *delimiter;

    cmd_table_node = ast->cmd_tables;
    while (cmd_table_node)
    {
        cmd_table = (t_cmd_table *)cmd_table_node->content;
        exec_cmd_table(cmd_table);

        delimiter = cmd_table->delimiter;
        // Ignore logical operators for now
        cmd_table_node = cmd_table_node->next;
    }
}

/* ************************************************************************** */
/*                 🏆 EXECUTE A SINGLE COMMAND TABLE                          */
/* ************************************************************************** */
void exec_cmd_table(t_cmd_table *cmd_table)
{
    t_list *cmds;
    int i;

    cmds = cmd_table->cmds;
    cmd_table->nb_cmds = ft_lstsize(cmds);
    i = 0;

    while (cmds)
    {
		exec_cmd((t_cmd *)cmds->content, cmd_table, i);
        cmds = cmds->next;
        i++;
    }

    exec_parent(&cmd_table->pids); // Wait for processes
}

/* ************************************************************************** */
/*                  🏆 EXECUTE A SINGLE COMMAND                              */
/* ************************************************************************** */
void exec_cmd(t_cmd *cmd, t_cmd_table *cmd_table, int process_index)
{
    if (cmd->tokens != NULL)
    {
        if (is_builtin(cmd))
            exec_builtin(cmd);
        else
            exec_external(cmd, cmd_table, process_index);
    }
}

/* ************************************************************************** */
/*              🏆 EXECUTE BUILT-IN COMMANDS (NO FORKING REQUIRED)             */
/* ************************************************************************** */
void exec_builtin(t_cmd *cmd)
{
    char *program = (char *)cmd->tokens->content;

    if (strcmp(program, "exit") == 0)
        exit(EXIT_SUCCESS);
    else if (strcmp(program, "echo") == 0)
        shell_echo(convert_list_to_arr(cmd->tokens->next));
    else if (strcmp(program, "cd") == 0)
        shell_cd((char *)cmd->tokens->next->content);
    else if (strcmp(program, "pwd") == 0)
        shell_pwd();
}

/* ************************************************************************** */
/*              🏆 EXECUTE EXTERNAL PROGRAMS (REQUIRES FORKING)                */
/* ************************************************************************** */
void exec_external(t_cmd *cmd, t_cmd_table *cmd_table, int process_index)
{
    char **tokens = convert_list_to_arr(cmd->tokens);
    pid_t pid = fork();

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
        ft_lstadd_back(&cmd_table->pids, ft_lstnew((void *)(intptr_t)pid));
        free(tokens);
    }
}

/* ************************************************************************** */
/*              🏆 WAIT FOR ALL CHILD PROCESSES TO FINISH                      */
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
/*              🏆 HELPER FUNCTIONS FOR COMMAND EXECUTION                      */
/* ************************************************************************** */

/* Check if command is a built-in */
int is_builtin(t_cmd *cmd)
{
    char *program = (char *)cmd->tokens->content;
    return (strcmp(program, "exit") == 0 ||
            strcmp(program, "echo") == 0 ||
            strcmp(program, "cd") == 0 ||
            strcmp(program, "pwd") == 0);
}

/* Convert linked list to NULL-terminated array */
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
