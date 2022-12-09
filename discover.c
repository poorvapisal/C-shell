#include "global.h"

int check_file(char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return -1;
    else
    {
        if (S_ISREG(statbuf.st_mode) != 0)
            return 1;
        else
            return 0;
    }
}

int check_directory(char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return -1;

    else
    {
        if (S_ISDIR(statbuf.st_mode) != 0)
            return 1;

        else
            return 0;
    }
}

void recursive_find(char *path, char *path_to_find, int p_flag, int d_flag, int f_flag)
{

    char filepath[1000] = "";
    struct dirent *dp;
    DIR *dir = opendir(path);
    getcwd(current_dir, MAX_DIR_PATH_LEN);
    char paths[MAX_DIR_PATH_LEN] = "";

    if (!dir)
        return;

    // printf("finding...\n");

    while ((dp = readdir(dir)) != NULL)
    {
        strcpy(paths, path);
        strcat(paths, "/");
        strcat(paths, dp->d_name);
        // printf("%s\n", paths);

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if (p_flag == 0)
            {
                if ((d_flag == 0 && f_flag == 0) || (d_flag == 1 && f_flag == 1))
                {
                    if (strcmp(path, current_dir) == 0)
                        printf("./%s\n", dp->d_name);

                    else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                        printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                    else
                        printf(".%s/%s\n", path, dp->d_name);
                }
                else if (d_flag == 0 && f_flag == 1)
                {
                    if (check_directory(paths) == 0 || check_file(paths) == 1)
                    {
                        if (strcmp(path, current_dir) == 0)
                            printf("./%s\n", dp->d_name);

                        else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                            printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                        else
                            printf(".%s/%s\n", path, dp->d_name);
                    }
                }
                else if (d_flag == 1 && f_flag == 0)
                {
                    if (check_directory(paths) == 1 || check_file(paths) == 0)
                    {
                        if (strcmp(path, current_dir) == 0)
                            printf("./%s\n", dp->d_name);

                        else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                            printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                        else
                            printf(".%s/%s\n", path, dp->d_name);
                    }
                }
            }

            else
            {
                if ((d_flag == 0 && f_flag == 0) || (d_flag == 1 && f_flag == 1))
                {
                    if (strcmp(path_to_find, dp->d_name) == 0)
                    {
                        if (strcmp(path, current_dir) == 0)
                            printf("./%s\n", dp->d_name);

                        else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                            printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                        else
                            printf(".%s/%s\n", path, dp->d_name);
                    }
                }
                else if (d_flag == 0 && f_flag == 1)
                {
                    if (strcmp(path_to_find, dp->d_name) == 0 && check_directory(paths) == 0)
                    {
                        if (strcmp(path, current_dir) == 0)
                            printf("./%s\n", dp->d_name);

                        else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                            printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                        else
                            printf(".%s/%s\n", path, dp->d_name);
                    }
                }
                else if (d_flag == 1 && f_flag == 0)
                {
                    // printf("check:%d\n", check_directory(paths));

                    if (strcmp(path_to_find, dp->d_name) == 0 && (check_directory(paths) == 1))
                    {
                        // printf("NOPE\n");
                        if (strcmp(path, current_dir) == 0)
                            printf("./%s\n", dp->d_name);

                        else if (strncmp(current_dir, path, strlen(current_dir)) == 0)
                            printf(".%s/%s\n", &path[strlen(current_dir)], dp->d_name);

                        else
                            printf(".%s/%s\n", path, dp->d_name);
                    }
                }
            }

            strcpy(filepath, path);
            strcat(filepath, "/");
            strcat(filepath, dp->d_name);

            recursive_find(filepath, path_to_find, p_flag, d_flag, f_flag);
        }
    }

    closedir(dir);
}

void discover(char *tokens[], long long int num_arg)
{

    int f_flag = 0;
    int d_flag = 0;
    int p_flag = 0;
    int find = 0;
    // recursive_find(root_dir);
    char path_to_find[MAX_DIR_PATH_LEN] = "";

    int error_flag = 0;
    char paths[MAX_DIR_PATH_LEN] = "";
    char cwd[MAX_DIR_PATH_LEN] = "";
    getcwd(cwd, MAX_DIR_PATH_LEN);

    for (long long int i = 1; i < num_arg; i++)
    {
        if (strcmp(tokens[i], "-f") == 0)
            f_flag = 1;

        else if (strcmp(tokens[i], "-d") == 0)
            d_flag = 1;

        else
        {
            if (tokens[i][0] == '\"')
            {
                find = 1;
                strcpy(path_to_find, ++tokens[i]);
                char *p = path_to_find;
                p[strlen(p) - 1] = '\0';
            }
            else
            {
                p_flag = 1;
                if (tokens[i] == NULL || strcmp(tokens[i], ".") == 0)
                    strcat(paths, cwd);

                else if (strcmp(tokens[i], "~") == 0)
                    strcat(paths, root_dir);

                else if (strcmp(tokens[i], "..") == 0)
                {
                    char path[MAX_DIR_PATH_LEN];
                    strcpy(path, cwd);
                    char *temp;
                    temp = strrchr(path, '/'); // Get the pointer to char token

                    if (temp != NULL)
                        *temp = '\0';

                    strcat(paths, path);
                }
                else
                {
                    if (check_directory(tokens[i]) == 1 || check_file(tokens[i]) == 1)
                    {
                        // strcat(paths, cwd);
                        // strcat(paths, "/");
                        strcat(paths, tokens[i]);
                    }
                    else
                    {
                        perror(tokens[i]);
                        error_flag = 1;
                    }
                }
            }
        }
    }
    if (p_flag == 0)
        strcat(paths, cwd);

    // printf("f_flag: %d\n", f_flag);
    // printf("d_flag: %d\n", d_flag);
    // printf("p_flag: %d\n", find);
    // printf("path: %s\n", paths);
    // printf("finding: %s\n\n\n", path_to_find);

    recursive_find(paths, path_to_find, find, d_flag, f_flag);
}