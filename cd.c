#include "global.h"

void cd(char *tokens[], long long int num_arg)
{
    char prev_directory[MAX_DIR_PATH_LEN];
    getcwd(current_dir, MAX_DIR_PATH_LEN);
    int success = 2;

    if (num_arg <= 2)
    {
        // printf("tokens[1]=%s\n", tokens[1]);
        if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0)
        {
            // printf("1\n");
            getcwd(prev_directory, MAX_DIR_PATH_LEN);
            success = chdir(root_dir);
        }

        else if (strcmp(tokens[1], "..") == 0)
        {
            // printf("2\n");
            getcwd(prev_directory, MAX_DIR_PATH_LEN);
            success = chdir("../");
        }

        else if (strcmp(tokens[1], ".") == 0)
        {
            // printf("3\n");
            getcwd(prev_directory, MAX_DIR_PATH_LEN);
        }

        else if (strcmp(tokens[1], "-") == 0)
        {
            // printf("4\n");
            if (strcmp(prev_directory, "") == 0)
                printf("cd: OLDPWD not set\n");

            else
            {
                char curr_dir[MAX_DIR_PATH_LEN];
                getcwd(curr_dir, MAX_DIR_PATH_LEN);

                printf("%s\n", prev_directory);
                success = chdir(prev_directory);
                strcpy(prev_directory, curr_dir);
            }
        }

        else
        {
            // printf("5\n");
            getcwd(prev_directory, MAX_DIR_PATH_LEN);
            success = chdir(tokens[1]);
        }
    }

    else
        printf("Error: cd: too many arguments\n");

    if (success == -1)
        perror("Error");

    getcwd(current_dir, MAX_DIR_PATH_LEN);
}