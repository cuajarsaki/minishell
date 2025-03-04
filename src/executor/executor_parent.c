#include "executor.h"

int exec_parent(t_list **pids)
{
    int status;
    t_list *pids_now;

    pids_now = *pids;
    while (pids_now)
    {
        pid_t pid = (pid_t)(intptr_t)pids_now->content;
        waitpid(pid, &status, 0);
        pids_now = pids_now->next;
        
    }
    // // debugs
    // printf("debugs:(in exec_parent)\n");
    // // debugs
    // printf("debugs:Exit status before WIFEXITED: %d\n", status);
    // if (WIFEXITED(status))
    //     // debugs
    //     printf("debugs:Exit status: %d\n", WEXITSTATUS(status));

    // TODO:change WEXITSTATUS later!
    return (WEXITSTATUS(status));
}