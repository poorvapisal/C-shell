#include "global.h"

struct bg_process
{
    char name[100];
    pid_t pid;
    int status; // 1 if running, 0 if terminated
};

void handler(int sig)
{
    pid_t pid;
    int status;
    // printf("process name:%s\n", proc[num_of_proc].name);

    pid = waitpid(-1, &status, WNOHANG);
    // printf("status:%d\n", status);

    if (pid > 0)
    {
        for (int i = 0; i < num_of_proc; i++)
        {
            if (proc[i].pid == pid && proc[i].status == 1)
            {
                if (WIFEXITED(status))
                    printf("\n%s with pid %d has exited normally\n", proc[i].name, proc[i].pid);

                else
                    printf("\n%s with pid %d has exited abnormally\n", proc[i].name, proc[i].pid);

                proc[i].status = 0;
                delete_process(proc[i].pid);
                fflush(stdout);

                break;
            }
        }
    }
    return;
}

void background(char *tokens[])
{
    pid_t child_pid;
    int stats;

    pid_t back_pid;
    child_pid = fork();

    if (child_pid < 0)
        printf("Error: fork failed\n");

    else if (child_pid == 0)
    {
        setpgid(0, 0);

        int result = execvp(tokens[0], tokens);

        if (result < 0)
            printf("Error exec failed: command does not exist\n");

        exit(0);
    }
    else
    {
        printf("[%d] %d\n", num_of_proc + 1, child_pid);

        // if (proc[num_of_proc].name == NULL)
        strcpy(proc[num_of_proc].name, tokens[0]);
        // printf("process name:%s\n", proc[num_of_proc].name);
        // printf("DONE\n");
        proc[num_of_proc].pid = child_pid;
        proc[num_of_proc].status = 1;
        proc[num_of_proc].job_num = total_jobs + 1;
        total_jobs++;
        num_of_proc++;

        signal(SIGCHLD, handler);
    }
}
