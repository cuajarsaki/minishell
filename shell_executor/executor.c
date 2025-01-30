#include "../shell.h"
#include <unistd.h>
#include <sys/wait.h> 
#include <ctype.h>

#include <stdio.h>

#define RED "\033[1;31m"
#define RESET "\033[0m"

extern char **environ;

void expand_arguments(char **args, t_env *env_list)
{
    if (!args) return;

    int i = 0;
    while (args[i])
    {
        char *arg = args[i];

        // Check if argument contains a variable
        if (strchr(arg, '$'))
        {
            char expanded_arg[4096] = {0}; // Buffer to store expanded string
            int j = 0;

            while (arg[j])
            {
                if (arg[j] == '$') // Variable found
                {
                    j++; // Move past '$'
                    char var_name[256] = {0};
                    int k = 0;

                    while (arg[j] && (isalnum(arg[j]) || arg[j] == '_')) // Capture variable name
                        var_name[k++] = arg[j++];
                    var_name[k] = '\0'; // Null-terminate the variable name

                    char *value = get_env_value(env_list, var_name);
                    if (value)
                    {
                        strcat(expanded_arg, value); // Append expanded value
                    }
                    else
                    {
                        strcat(expanded_arg, "$"); // Keep unexpanded if not found
                        strcat(expanded_arg, var_name);
                    }
                }
                else
                {
                    strncat(expanded_arg, &arg[j], 1); // Copy non-variable characters
                    j++;
                }
            }

            free(args[i]);  
            args[i] = strdup(expanded_arg); // Store expanded value
        }

        i++;
    }
}


int execute_builtin(ASTreeNode *node, t_env **env_list)
{
    printf("\n[DEBUG] Command: %s\n", node->command);
    for (int i = 0; node->args && node->args[i]; i++)
        printf("[DEBUG] Arg[%d]: %s\n", i, node->args[i]);

    if (ft_strcmp(node->command, "export") != 0) 
    { // Skip expansion for export
        expand_arguments(node->args, *env_list);
    }

    if (ft_strcmp(node->command, "clear") == 0) {
        shell_clear();
        return 1;
    }
    if (ft_strcmp(node->command, "ls") == 0) {
        shell_ls(node->args);
        return 1;
    }
    if (ft_strcmp(node->command, "pwd") == 0) {
        shell_pwd();
        return 1;
    }
    if (ft_strcmp(node->command, "cd") == 0) {
        shell_cd(node->args ? node->args[0] : NULL);
        return 1;
    }
    if (ft_strcmp(node->command, "echo") == 0) {
        shell_echo(node->args);
        return 1;
    }
    if (ft_strcmp(node->command, "env") == 0) {
        print_env_list(*env_list);
        return 1;
    }
    if (ft_strcmp(node->command, "export") == 0) {
        if (node->args && node->args[0]) {
            printf(RED "DEBUG: export command entered\n" RESET);

            char *arg = strdup(node->args[0]);
            if (!arg) {
                perror(RED "strdup failed" RESET);
                return 1;
            }

            printf(RED "DEBUG: Argument received: %s\n" RESET, arg);

            char *equal_sign = strchr(arg, '=');
            if (!equal_sign) {
                printf(RED "DEBUG: Equal sign not found\n" RESET);
                printf(RED "❌ [ERROR] Invalid format! Use: export VAR=value\n" RESET);
                free(arg);
                return 1;
            }

            *equal_sign = '\0';
            char *key = arg;
            char *value = equal_sign + 1;

            printf(RED "DEBUG: Key extracted: %s\n" RESET, key);
            printf(RED "DEBUG: Value extracted (before processing): %s\n" RESET, value);

            // Correctly handle quoted values with spaces
            if ((*value == '"' || *value == '\'') && value[strlen(value) - 1] == *value) {
                printf(RED "DEBUG: Value detected as quoted\n" RESET);
                value++; // Skip opening quote
                value[strlen(value) - 1] = '\0'; // Remove closing quote
            } else {
                printf(RED "DEBUG: Value is NOT quoted\n" RESET);
            }

            printf(RED "DEBUG: Value after processing: %s\n" RESET, value);

            set_env_value(env_list, key, value);
            printf(RED "DEBUG: Environment variable set successfully\n" RESET);

            free(arg);
        }
        return 1;
    }
    if (ft_strcmp(node->command, "unset") == 0) {
        if (node->args && node->args[0])
            unset_env_value(env_list, node->args[0]);
        return 1;
    }
    return 0; // Not a built-in command
}

void execute_syntax_tree(ASTreeNode *node, t_env *env_list)
{
    if (node == NULL)
        return;

    // Handle built-in commands
    if (execute_builtin(node, &env_list))
        return;

    // If not built-in, execute as external command
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        char **envp = convert_env_list_to_array(env_list); // Convert linked list to char** for execve
        if (execve(node->command, node->args, envp) == -1) {
            perror("execve");
            free_env_array(envp);
            exit(1);
        }
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (status != 0) {
            fprintf(stderr, "Execution error: %s\n", node->command);
        }
    }
    else
    {
        perror("fork");
    }
}
