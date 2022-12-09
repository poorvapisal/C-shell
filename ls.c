#include "global.h"

int is_file(char *path)
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

int is_directory(char *path)
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

void l_print(char path[], char file[])
{
    struct stat file_stats;
    stat(path, &file_stats);

    char permissions[11];
    permissions[0] = S_ISDIR(file_stats.st_mode) ? 'd' : '-';
    permissions[1] = (file_stats.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (file_stats.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (file_stats.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (file_stats.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (file_stats.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (file_stats.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (file_stats.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (file_stats.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (file_stats.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    long int noOfLinks;
    noOfLinks = file_stats.st_nlink;

    // getting owner and group name
    struct passwd *owner = getpwuid(file_stats.st_uid);
    struct group *group = getgrgid(file_stats.st_gid);

    // time of last modification
    char date[100];
    char year_or_time[20];
    time_t seconds;

    // if time of last modification is over 6 months print year instead of time
    time(&seconds);

    if ((seconds - file_stats.st_mtime) > 15778800)
    {
        strftime(year_or_time, 10, "%Y", localtime(&file_stats.st_mtime));
    }
    else
    {
        strftime(year_or_time, 10, "%H:%M", localtime(&file_stats.st_mtime));
    }

    strftime(date, 90, "%b %d", localtime(&file_stats.st_mtime));

    printf("%s %ld %s %s %ld %s %s %s\n", permissions, noOfLinks, owner->pw_name, group->gr_name, file_stats.st_size, date, year_or_time, file);
}

void ls(char *tokens[], long long int num_arg)
{
    // printf("%lld\n", num_arg);
    int l_flag = 0;
    int a_flag = 0;
    int ls_flag = 0;
    int error_flag = 0;
    char paths[MAX_DIR_PATH_LEN] = "";
    long int num_of_paths = 0;
    char cwd[MAX_DIR_PATH_LEN];
    getcwd(cwd, MAX_DIR_PATH_LEN);

    for (long long int i = 1; i < num_arg; i++)
    {
        if (strcmp(tokens[i], "-l") == 0)
            l_flag = 1;

        else if (strcmp(tokens[i], "-a") == 0)
            a_flag = 1;

        else if (strcmp(tokens[i], "-la") == 0 || strcmp(tokens[i], "-al") == 0)
        {
            l_flag = 1;
            a_flag = 1;
        }
        else
        {
            // printf("ls enter\n");
            if (tokens[i] == NULL || strcmp(tokens[i], ".") == 0)
            {
                // printf("HERE\n");
                ls_flag = 1;
                strcat(paths, cwd);
                strcat(paths, " ");
                num_of_paths++;
            }
            else if (strcmp(tokens[i], "~") == 0)
            {
                ls_flag = 1;
                strcat(paths, root_dir);
                strcat(paths, " ");
                num_of_paths++;
            }
            else if (strcmp(tokens[i], "..") == 0)
            {
                char path[MAX_DIR_PATH_LEN];
                strcpy(path, cwd);
                char *temp;
                temp = strrchr(path, '/'); // Get the pointer to char token

                if (temp != NULL)
                    *temp = '\0';

                strcat(paths, path);
                strcat(paths, " ");
                num_of_paths++;
            }
            else
            {
                if (is_directory(tokens[i]) == 1 || is_file(tokens[i]) == 1)
                {
                    strcat(paths, tokens[i]);
                    strcat(paths, " ");
                    num_of_paths++;
                }
                else
                {
                    perror(tokens[i]);
                    error_flag = 1;
                }
            }
        }
    }

    if (error_flag == 0 && num_of_paths == 0)
    {
        strcat(paths, cwd);
        num_of_paths++;
    }

    char *sep_paths[num_of_paths + 1];
    char *pathStrPtr;

    sep_paths[0] = strtok_r(paths, " ", &pathStrPtr);

    long int count = 0;
    while (sep_paths[count] != NULL && count < num_of_paths - 1)
    {
        count++;
        sep_paths[count] = strtok_r(NULL, " ", &pathStrPtr);
    }

    for (long int i = 0; i < num_of_paths; i++)
    {
        if (is_file(sep_paths[i]) == 1)
        {
            if (l_flag == 1)
                l_print(sep_paths[i], sep_paths[i]);
            else
                printf("%s\n", sep_paths[i]);
        }

        else if (is_directory(sep_paths[i]) == 1)
        {
            // printf("ls path: %s\n", sep_paths[i]);
            DIR *file_stream = opendir(sep_paths[i]);
            struct dirent *fileslist;

            long int block_size = 0;

            if (num_of_paths > 1)
                printf("%s:\n", sep_paths[i]);

            while ((fileslist = readdir(file_stream)) != NULL)
            {
                struct stat files;

                if (a_flag == 0 && fileslist->d_name[0] == '.')
                    continue;

                if (l_flag == 1)
                {
                    // printf("EXISTS\n");
                    char relative_path[1000] = "";
                    strcpy(relative_path, sep_paths[i]);
                    strcat(relative_path, "/");
                    strcat(relative_path, fileslist->d_name);

                    stat(relative_path, &files);
                    block_size += files.st_blocks;
                    // printf("rel path: %s\n", relative_path);
                }
            }

            if (l_flag == 1)
                printf("total %ld\n", block_size / 2);

            DIR *dir_stream = opendir(sep_paths[i]);
            struct dirent *dir_entry;

            if (dir_stream == NULL)
                perror("Error");

            else
            {
                while ((dir_entry = readdir(dir_stream)) != NULL)
                {
                    struct stat files;

                    if (a_flag == 0 && dir_entry->d_name[0] == '.')
                        continue;

                    if (l_flag == 1)
                    {
                        // printf("HERE\n");
                        char relative_path[1000] = "";
                        strcpy(relative_path, sep_paths[i]);
                        strcat(relative_path, "/");
                        strcat(relative_path, dir_entry->d_name);

                        if (!is_file(dir_entry->d_name))
                            blue();
                        else if (!is_directory(dir_entry->d_name) && !is_file(dir_entry->d_name))
                            green();

                        l_print(relative_path, dir_entry->d_name);
                        reset();
                    }

                    else
                    {
                        if (!is_file(dir_entry->d_name))
                            blue();
                        else if (!is_directory(dir_entry->d_name) && !is_file(dir_entry->d_name))
                            green();
                        printf("%s\n", dir_entry->d_name);
                        reset();
                    }
                }

                if (i < num_of_paths - 1)
                    printf("\n");
            }
        }
    }
}