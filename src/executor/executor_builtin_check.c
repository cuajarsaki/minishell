#include "executor.h"

int is_builtin(t_cmd *cmd)
{
    char *program = (char *)cmd->tokens->content;

    /*
        NOTE: Currently, this returns '0' for these commands,
        meaning "not recognized as builtin." If you want them
        recognized as built-ins, return 1 for matches instead.
    */
    if (!ft_strcmp(program, "echo") ||
        !ft_strcmp(program, "cd") ||
        !ft_strcmp(program, "pwd") ||
        !ft_strcmp(program, "export") ||
        !ft_strcmp(program, "unset") ||
        !ft_strcmp(program, "env") ||
        !ft_strcmp(program, "exit"))
    {
        return 1; // <--- This effectively says "Not builtin" in your logic
    }
    return 0; // <--- Everything else also returns "Not builtin"
}