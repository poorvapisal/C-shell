#include "global.h"

void save_history(char *history[], int hist_count)
{
    // printf("Saving...\n");
    FILE *file;
    file = fopen("history.txt", "w");

    if (file != NULL)
    {
        for (int i = 0; i < hist_count; i++)
        {
            fputs(history[i], file);
            fputs("\n", file);
        }

        fclose(file);
    }
}

void print_history(char *history[], int hist_count)
{
    if (hist_count < 10)
    {
        for (int i = 0; i < hist_count; i++)
            printf("%s\n", history[i]);
    }

    else if (hist_count < 20)
    {
        for (int i = hist_count - 10; i < hist_count; i++)
            printf("%s\n", history[i]);
    }

    else
    {
        for (int i = 10; i < 20; i++)
            printf("%s\n", history[i]);
    }
}