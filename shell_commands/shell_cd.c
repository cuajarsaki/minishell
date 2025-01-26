#include "../shell.h"

void shell_cd(char *path)
{
	if (path == NULL || ft_strcmp(path, "") == 0) {
		char *home = getenv("HOME");
		if (home == NULL) {
			write(2, "cd: HOME not set\n", 17);
			return;
		}
		path = home;
	}
	if (chdir(path) != 0) {
		perror("cd");
	} else {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			write(1, "Changed directory to: ", 22);
			write(1, cwd, ft_strlen(cwd));
			write(1, "\n", 1);
		} else {
			perror("getcwd");
		}
	}
}