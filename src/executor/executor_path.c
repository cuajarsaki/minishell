#include "executor.h"

char *find_executable_in_path(const char *cmd)
{
    char *path_env = getenv("PATH");
    if (!path_env || ft_strlen(path_env) == 0)
    {
        return NULL; // No PATH environment variable
    }

    char *paths = ft_strdup(path_env);
    char *dir = ft_strtok(paths, ":");

    while (dir)
    {
        char full_path[1024];
        ft_snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

		size_t dir_len = strlen(dir);
		size_t cmd_len = strlen(cmd);

		memcpy(full_path, dir, dir_len);        // Copy `dir`
		full_path[dir_len] = '/';               // Insert '/'
		memcpy(full_path + dir_len + 1, cmd, cmd_len); // Copy `cmd`
		full_path[dir_len + cmd_len + 1] = '\0'; // Null-terminate
		
		if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return ft_strdup(full_path); // Return a valid command path
        }
        dir = ft_strtok(NULL, ":");
    }

    free(paths);
    return NULL;
}

