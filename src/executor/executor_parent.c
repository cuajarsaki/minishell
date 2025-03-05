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

    if ((status & 0x7F) == 0)
        return (status >> 8) & 0xFF;
    else if (status & 0x7F)
        return 128 + (status & 0x7F);
    
    return 1;
}