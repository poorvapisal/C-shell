#include "global.h"

int pid = 0;
char filename[MAX_DIR_PATH_LEN];
char buf[MAX_DIR_PATH_LEN];

void readfile()
{
    sprintf(filename, "/proc/%d/exe", pid);
    if (readlink(filename, buf, MAX_DIR_PATH_LEN) == -1)
    {
        printf("Error in obtaining process executable path.\n");
        return;
    }

    char newexe[MAX_DIR_PATH_LEN] = "~";
    if (!(strncmp(buf, root_dir, strlen(root_dir))))
    {
        strcat(newexe, buf + strlen(root_dir));
    }
    else
    {
        strcpy(newexe, buf);
    }
    printf("Executable Path: %s\n", newexe);
}

void pinfo(char tokens[], long long int num_arg)
{
    if (num_arg > 2)
    {
        printf("pinfo: Invalid syntax too many arguments\n");
        return;
    }
    else
    {
        char *token = tokens;

        if (num_arg == 1)
            pid = getpid();

        else
            pid = atoi(strtok_r(token, " ", &token));

        // printf("pid=%d\n", pid);

        char status;
        int data1, data2, data3, data4;
        int num1, num2;
        long unsigned memory;

        sprintf(filename, "/proc/%d/stat", pid);

        FILE *file = fopen(filename, "r");

        if (!file)
            printf("Error in obtaining process stats\n");
        else
        {
            printf("pid: %d\n", pid);
            fscanf(file, "%d %s %c %d %d %d %d %d", &data1, buf, &status, &data2, &num1, &data3, &data4, &num2);
            printf("Process Status: %c", status);

            if (num1 == num2)
                printf("+\n");
            else
                printf("\n");

            for (int i = 0; i < 14; i++)
                fscanf(file, "%s", buf);

            fscanf(file, "%lu", &memory);
            printf("memory: %lu\n", memory);
        }

        readfile();
    }
}