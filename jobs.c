#include "global.h"

int compare(const void *a, const void *b)
{
    struct process *name1 = (struct process *)a;
    struct process *name2 = (struct process *)b;

    return strcmp(name1->name, name2->name);
}

void check(int s_flag, int r_flag)
{
    for (int i = 0; i < num_of_proc; i++)
    {
        char check_string[10005] = "";
        char pid_str[50];
        sprintf(pid_str, "%d", proc[i].pid);

        char process_file_path[10000];
        sprintf(process_file_path, "/proc/%d/stat", proc[i].pid);

        FILE *fp = fopen(process_file_path, "r");

        if (fp == NULL)
        {
            perror("stat file error:");
            continue;
        }

        char processState[1000];

        char *strptr;
        char fileinfo[10005];
        fgets(fileinfo, 10000, fp);

        char *infoblock[100];

        infoblock[0] = strtok_r(fileinfo, " \t", &strptr);

        long long int count = 0;

        while (infoblock[count] != NULL)
        {
            count++;
            infoblock[count] = strtok_r(NULL, " \t", &strptr);

            if (count == 2)
            {
                if (strcmp(infoblock[count], "T") == 0)
                {
                    strcpy(processState, "Stopped");
                }
                else
                {
                    strcpy(processState, "Running");
                }
                break;
            }
        }

        fclose(fp);

        if (s_flag == 0 && r_flag == 0)
            continue;

        if (r_flag == 1)
        {
            if ((strcmp(processState, "Running") == 0))
                printf("[%d] %s %s [%d]\n", proc[i].job_num, processState, proc[i].name, proc[i].pid);
        }

        if (s_flag == 1)
        {
            if ((strcmp(processState, "Stopped") == 0))
                printf("[%d] %s %s [%d]\n", proc[i].job_num, processState, proc[i].name, proc[i].pid);
        }
    }
}

void jobs(char *tokens[], long long int num_arg)
{
    int r_flag = 0;
    int s_flag = 0;

    if (num_arg > 1)
    {
        for (long long int i = 1; i < num_arg; i++)
        {
            if (strcmp(tokens[i], "-rs") == 0 || strcmp(tokens[i], "-sr") == 0)
            {
                r_flag = 1;
                s_flag = 1;
                break;
            }

            else if (strcmp(tokens[i], "-r") == 0)
                r_flag = 1;

            else if (strcmp(tokens[i], "-s") == 0)
                s_flag = 1;
        }
    }

    else
    {
        r_flag = 1;
        s_flag = 1;
    }

    qsort(proc, num_of_proc, sizeof(struct process), compare);
    check(s_flag, r_flag);
}

void fg(char *command[], long long int argnum)
{
    int process_exists = 0;
    if (argnum != 2)
    {
        printf("Usage: fg <job_serial_num>. Run the command 'jobs' to find the serial number.\n");
        return;
    }
    char *token;
    // char *rem = command;
    int job_no = atoi(command[1]);

    int pid = 0;

    if (job_no < 1 || job_no > num_of_proc)
        pid = -1;

    else
    {
        for (int i = 0; i < num_of_proc; i++)
        {
            if (proc[i].job_num == job_no)
                pid = proc[i].pid;
        }
    }

    int index = -1;
    int i = 0;
    for (i = 0; i < num_of_proc; i++)
    {
        if (proc[i].job_num == job_no)
            index = i;
    }

    for (int j = 0; j < num_of_proc; j++)
    {
        // printf("HELLO\n");
        if (proc[j].job_num == job_no)
        {
            setpgid(pid, getpgid(0));
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            int result = tcsetpgrp(STDIN_FILENO, proc[j].pid);
            kill(proc[j].pid, SIGCONT);

            if (result == -1)
            {
                perror("tcsetpgrp error");
                exit(EXIT_FAILURE);
            }

            process_exists = 1;

            fg_process.pid = proc[j].pid;
            strcpy(fg_process.name, proc[j].name);

            kill(proc[j].pid, SIGCONT);
            delete_process(proc[j].pid);

            waitpid(-1, NULL, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
    }
    if (process_exists == 0)
        printf("Error: that process does not exist\n");

    return;
}

void bg(char *command[], long long int argnum)
{
    int process_exists = 0;
    if (argnum != 2)
    {
        printf("Usage: bg <job_serial_num>. Run the command 'jobs' to find the serial number.\n");
        return;
    }

    int job_number = atoi(command[1]);
    // printf("job=%d\n", job_number);

    for (int i = 0; i < num_of_proc; i++)
    {
        if (proc[i].job_num == job_number)
        {
            kill(proc[i].pid, SIGTTIN);
            kill(proc[i].pid, SIGCONT);
            // printf("HELLO\n");
            process_exists = 1;
        }
    }

    if (process_exists == 0)
    {
        printf("Error: process does not exist\n");
        return;
    }
}