#include "global.h"

void delete_process(int pid)
{
    // printf("DELETING\n");
    int process_exists = 0;
    for (int i = 0; i < num_of_proc; i++)
    {
        if (proc[i].pid == pid)
        {
            process_exists = 1;
            for (int j = i; j < num_of_proc - 1; j++)
            {
                proc[j] = proc[j + 1];
            }
            num_of_proc--;
        }
    }
    if (process_exists == 0)
    {
        printf("Error: could not be deleted, process doesn't exist\n");
    }
}
