#include "executor.h"

char *find_executable_in_path(const char *cmd, t_env *env_list)
{

    char *path_env = get_env_value(env_list, "PATH");
    if (!path_env || ft_strlen(path_env) == 0)
    {
        return NULL; 
    }

    char *paths = ft_strdup(path_env); 
    char *dir = ft_strtok(paths, ":");

    while (dir)
    {
        char full_path[1024];
        ft_snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        size_t dir_len = strlen(dir);
        size_t cmd_len = strlen(cmd);

        memcpy(full_path, dir, dir_len);               
        full_path[dir_len] = '/';                  
        memcpy(full_path + dir_len + 1, cmd, cmd_len);
        full_path[dir_len + cmd_len + 1] = '\0';    

        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return ft_strdup(full_path); 
        }
        dir = ft_strtok(NULL, ":");
    }

    free(paths);
    return NULL;
}
