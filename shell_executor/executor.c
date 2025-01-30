#include "../shell.h"
#include <unistd.h>
#include <sys/wait.h> 

extern char **environ;

void expand_arguments(char **args, t_env *env_list)
{
    if (!args) return;

    int i = 0;
    while (args[i])
    {
        if (args[i][0] == '$')
        {
            char *value = get_env_value(env_list, args[i] + 1); // Get value from env_list
            if (value)
            {
                free(args[i]); 
                args[i] = strdup(value); 
            }
            else
            {
                printf("❌ [ERROR] Variable %s not found\n", args[i]);
            }
        }
        i++;
    }
}

int execute_builtin(ASTreeNode *node, t_env **env_list)
{
    expand_arguments(node->args, *env_list);

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
			char *arg = strdup(node->args[0]);
			char *key = strtok(arg, "=");
			char *value = strtok(NULL, "=");
			if (key && value) {
				set_env_value(env_list, key, value);
			}
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

// void execute_syntax_tree(ASTreeNode *node,  t_env *env_list) 
// {
//     if (node == NULL) return;

//     if (node->command) {
//         if (ft_strcmp(node->command, "clear") == 0) {
//             shell_clear();
//         } else if (ft_strcmp(node->command, "ls") == 0) {
//             shell_ls(node->args);
//         } else if (ft_strcmp(node->command, "pwd") == 0) {
//             shell_pwd();
//         } else if (ft_strcmp(node->command, "cd") == 0) {
//             shell_cd(node->args ? node->args[0] : NULL);
//         } else if (ft_strcmp(node->command, "echo") == 0) {
//             shell_echo(node->args);
//         }
// 		else if (strcmp(node->command, "env") == 0)
// 		{
// 			print_env_list(env_list);
// 		}
// 		else {
// 			// custom-implement //
// 			fprintf(stderr, "Command failed: %s\n", node->command);
// 		}
// 		// JOHAN: I HAVE TO CHECK LATER
// 			// custom-implement //
//         //     pid_t pid = fork();
//         //     if (pid == 0) {
//         //         if (execvp(node->command, node->args) == -1) {
//         //             perror("execvp");
//         //             exit(1); // Exit child process
//         //         }
//         //     } else if (pid < 0) {
//         //         perror("fork");
//         //     } else {
//         //         int status;
//         //         waitpid(pid, &status, 0);
//         //         if (status != 0) {
//         //             fprintf(stderr, "Command failed: %s\n", node->command);

//         //             // Clear buffer or reset state here if necessary
//         //         }
//         //     }
//         // }
//     }
// }
