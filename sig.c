#include "global.h"

void sig(char *argv[], long long int argc)
{
    if (argc != 3)
    {
        printf("2 arguments must be supplied for \"sig\": sig [job_no] [signal]\n");
        return;
    }
    int job_no = atoi(argv[1]);
    if (job_no <= 0 || job_no > total_jobs)
    {
        printf("Incorrect Job number. Aborting!\n");
        return;
    }

    int signal = atoi(argv[2]);
    if (!signal)
        return;

    int pid = 0;
    for (int i = 0; i < num_of_proc; i++)
    {
        if (proc[i].job_num == job_no)
        {
            pid = proc[i].pid;
            break;
        }
    }
    if (!pid)
    {
        printf("job with job number [%d] not found \n", job_no);
        return;
    }

    int ret = kill(pid, signal);
    if (ret < 0)
        perror("Error: ");
}