#include "global.h"

void echo(char *string[], long long int arg_count)
{
    char output[MAX_INPUT_CMD_LEN] = "";
    for (long long int i = 1; i < arg_count; i++)
    {
        strcat(output, string[i]);
        strcat(output, " ");
    }
    printf("%s\n", output);
}