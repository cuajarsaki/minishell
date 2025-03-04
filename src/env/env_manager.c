#include "../../shell.h"

// Global environment variable list
t_env *g_env = NULL;

void init_global_env(void)
{
    if (!g_env)
        g_env = init_env_list();
}

void free_global_env(void)
{
    free_env_list(g_env);
    g_env = NULL;
}
