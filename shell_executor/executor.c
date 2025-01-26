#include "../shell.h"
#include <unistd.h>
#include <sys/wait.h> 

void execute_syntax_tree(ASTreeNode *node) {
    if (node == NULL) return;

    if (node->command) {
        if (ft_strcmp(node->command, "clear") == 0) {
            shell_clear();
        } else if (ft_strcmp(node->command, "ls") == 0) {
            shell_ls(node->args);
        } else if (ft_strcmp(node->command, "pwd") == 0) {
            shell_pwd();
        } else if (ft_strcmp(node->command, "cd") == 0) {
            shell_cd(node->args ? node->args[0] : NULL);
        } else if (ft_strcmp(node->command, "echo") == 0) {
            shell_echo(node->args);
        } else {
			// custom-implement //
			fprintf(stderr, "Command failed: %s\n", node->command);
		}
		// JOHAN: I HAVE TO CHECK LATER
			// custom-implement //
        //     pid_t pid = fork();
        //     if (pid == 0) {
        //         if (execvp(node->command, node->args) == -1) {
        //             perror("execvp");
        //             exit(1); // Exit child process
        //         }
        //     } else if (pid < 0) {
        //         perror("fork");
        //     } else {
        //         int status;
        //         waitpid(pid, &status, 0);
        //         if (status != 0) {
        //             fprintf(stderr, "Command failed: %s\n", node->command);

        //             // Clear buffer or reset state here if necessary
        //         }
        //     }
        // }
    }
}
