#include "global.h"

void foreground(char *tokens[])
{
    pid_t child_pid;
    int stats;
    child_pid = fork();

    fg_process.pid = child_pid;
    strcpy(fg_process.name, tokens[0]);

    if (child_pid < 0)
        printf("Error: fork failed\n");

    else if (child_pid == 0)
    {
        int result = execvp(tokens[0], tokens);

        if (result < 0)
            printf("Error exec failed: command does not exist\n");

        exit(0);
    }
    else
        waitpid(child_pid, &stats, WUNTRACED);
}