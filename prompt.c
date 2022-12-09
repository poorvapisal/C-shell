#include "global.h"

void prompt()
{
    gethostname(hostname, _SC_HOST_NAME_MAX);
    getlogin_r(username, _SC_LOGIN_NAME_MAX);

    getcwd(current_dir, MAX_DIR_PATH_LEN);

    green();

    if (fore == 0)
    {
        if (strcmp(current_dir, root_dir) == 0)
            printf("<%s@%s:~>", username, hostname);

        else if (strncmp(root_dir, current_dir, strlen(root_dir)) == 0)
            printf("<%s@%s:~%s>", username, hostname, &current_dir[strlen(root_dir)]);

        else
            printf("<%s@%s:%s>", username, hostname, current_dir);
    }

    else
    {
        if (strcmp(current_dir, root_dir) == 0)
            printf("<%s@%s:~took %lds>", username, hostname, secs2 - secs1);

        else if (strncmp(root_dir, current_dir, strlen(root_dir)) == 0)
            printf("<%s@%s:~%s took %lds>", username, hostname, &current_dir[strlen(root_dir)], secs2 - secs1);

        else
            printf("<%s@%s:%s took %lds>", username, hostname, current_dir, secs2 - secs1);
    }

    reset();
}
