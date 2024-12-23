#include "../shell.h"
#include <unistd.h>
#include <sys/wait.h> 

void execute_syntax_tree(ASTreeNode *node) {
	if (node == NULL) return;

	if (node->left) execute_syntax_tree(node->left);
	if (node->right) execute_syntax_tree(node->right);

	if (node->command) {
		if (strcmp(node->command, "clear") == 0) {
			shell_clear();
		} else if (strcmp(node->command, "ls") == 0) {
			shell_ls(node->args);
		} else if (strcmp(node->command, "pwd") == 0) {
			shell_pwd();
		} else if (strcmp(node->command, "cd") == 0) {
			shell_cd(node->args ? node->args[0] : NULL);
		} else if (strcmp(node->command, "echo") == 0) {
			shell_echo(node->args);
		}
		else if (strcmp(node->command, "move") == 0)
		{
			move_cursor(10, 0);
		}
		 else if (strcmp(node->command, "man") == 0) {
			shell_man(node->args ? node->args[0] : NULL);
		} else {
			pid_t pid = fork();
			if (pid == 0) {
				if (execvp(node->command, node->args) == -1) {
					perror("execvp");
					_exit(1);
				}
			} else if (pid < 0) {
				perror("fork");
			} else {
				int status;
				waitpid(pid, &status, 0);
			}
		}
	}
}
