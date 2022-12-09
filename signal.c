#include "global.h"

void ctrl_c(int sig_num)
{
    // printf("CTRL-Z\n");
    pid_t curr_pid = getpid();

    if (fg_process.pid != -1)
        kill(fg_process.pid, SIGINT);

    signal(SIGINT, ctrl_c);
    return;
}

void ctrl_z(int sig_num)
{
    // printf("CTRL-Z\n");
    pid_t curr_pid = getpid();

    if (fg_process.pid != -1)
    {
        kill(fg_process.pid, SIGTTIN);
        kill(fg_process.pid, SIGTSTP);

        proc[num_of_proc].pid = fg_process.pid;
        proc[num_of_proc].job_num = total_jobs + 1;
        total_jobs++;
        strcpy(proc[num_of_proc].name, fg_process.name);

        num_of_proc++;
        return;
    }

    signal(SIGTSTP, ctrl_z);
    return;
}