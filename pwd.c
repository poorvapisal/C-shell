#include "global.h"

void pwd()
{
    if (getcwd(current_dir, MAX_DIR_PATH_LEN) == NULL)
        perror("getcwd() error: ");

    else
        printf("%s\n", current_dir);
}