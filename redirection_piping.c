#include "global.h"

int std_in;
int std_out;
char input[10000] = "";
char output[10000] = "";
int type[] = {0, 0};
int position[] = {0, 0};
char *command[10000];
char *pipe_commands[1000];
char *inputStrPtr;
int no_of_pipe_cmds = 0;

int piping_check(char *token[], long long int num_arg)
{
    for (long long int i = 0; token[i] != NULL; i++)
    {
        if (strcmp(token[i], "|") == 0)
        {
            return 1;
            break;
        }
    }

    return 0;
}

int redirection_check(char *token[], long long int num_arg, long long int *position)
{
    int flag = 0;
    int i = 0;

    for (int i = 0; i < num_arg; i++)
    {
        if (token[i] == NULL)
            break;

        if (strcmp(token[i], "<") == 0)
        {
            flag = 1;
            *position = i;
            break;
        }
        else if (strcmp(token[i], ">") == 0)
        {
            flag = 2;
            *position = i;
            break;
        }
        else if (strcmp(token[i], ">>") == 0)
        {
            flag = 3;
            *position = i;
            break;
        }
    }

    return flag;
}

void std()
{
    std_out = dup(STDOUT_FILENO);
    std_in = dup(STDIN_FILENO);
    type[0] = 0;
    type[1] = 0;
    position[0] = 0;
    position[1] = 0;
}

int input_outputset(char *token[], long long int num_arg)
{
    if (type[0] == 1 && type[1] == 0)
    {
        if ((num_arg - 1) == position[0])
        {
            printf("Error: No input file given\n");
            return 0;
        }
        strcpy(input, token[num_arg - 1]);
    }

    else if (type[1] > 0 && type[0] == 1)
    {
        if (position[0] < position[1])
        {
            if (token[position[1] - 1] == NULL)
            {
                printf("Error: No input file given\n");
                return 0;
            }
            else
                strcpy(input, token[position[1] - 1]);
        }
        else if (position[1] < position[0])
        {
            if ((num_arg - 1) == position[0])
            {
                printf("Error: No input file given\n");
                return 0;
            }
            else
                strcpy(input, token[num_arg - 1]);
        }
    }

    if (type[1])
    {
        // printf("%s\n", token[position[1] + 1]);
        if (token[position[1] + 1] == NULL)
        {
            // printf("%s\n", token[position[1] + 1]);
            printf("Error: No output file given\n");
            return 0;
        }
        else
            strcpy(output, token[position[1] + 1]);
    }

    return 1;
}

int red_child_process(long long int redirection_position)
{
    int status;
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork error: Child process not created\n");
        return 0;
    }
    else if (pid == 0)
    {
        if (type[0])
        {
            int fi = open(input, O_RDONLY);

            if (fi < 0)
            {
                perror("Input file could not be opened");
                exit(EXIT_SUCCESS);
            }

            dup2(fi, STDIN_FILENO);
            close(fi);
        }
        if (type[1])
        {
            int fo;

            if (type[1] == 1)
                fo = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            else if (type[1] == 2)
                fo = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if (fo < 0)
            {
                perror("Output file could not be opened");
                exit(EXIT_SUCCESS);
            }

            dup2(fo, STDOUT_FILENO);
            close(fo);
        }

        execute("", command, redirection_position);

        dup2(std_in, 0);
        close(std_in);
        dup2(std_out, 1);
        close(std_out);

        exit(EXIT_SUCCESS);
    }
    else
        while (wait(&status) != pid)
            ;

    return 1;
}

void redirection(char *token[], long long int arg_num, int redirection_flag, long long int redirection_position)
{
    std();

    for (long long int i = 0; i < arg_num; i++)
    {
        if (strcmp(token[i], "<") == 0)
        {
            type[0] = 1;
            position[0] = i;
        }
        else if (strcmp(token[i], ">") == 0)
        {
            type[1] = 1;
            position[1] = i;
        }
        else if (strcmp(token[i], ">>") == 0)
        {
            type[1] = 2;
            position[1] = i;
        }
    }

    int check = input_outputset(token, arg_num);
    if (check == 0)
        return;

    for (long long int i = 0; i < redirection_position; i++)
        command[i] = token[i];
    command[redirection_position] = NULL;

    int check_process = red_child_process(redirection_position);
    if (check_process == 0)
        return;
}

int piping_child_process()
{
    int fd[2], temp_fd = 0;
    pid_t pid;

    for (int m = 0; m < no_of_pipe_cmds; m++)
    {
        char *tokens[1000];
        long int k = 0;
        tokens[0] = strtok_r(pipe_commands[m], " \n", &inputStrPtr);

        while (tokens[k] != NULL)
        {
            k++;
            tokens[k] = strtok_r(NULL, " \n", &inputStrPtr);
        }

        if (pipe(fd) < 0)
        {
            perror("Pipe error");
            return 0;
        }

        pid = fork();

        if (pid < 0)
            perror("Fork error");

        else if (pid > 0)
        {
            wait(NULL);
            close(fd[1]);
            temp_fd = fd[0];
        }

        else if (pid == 0)
        {
            dup2(temp_fd, 0);

            if (m < (no_of_pipe_cmds - 1))
                dup2(fd[1], 1);

            close(fd[0]);
            execute("", tokens, k);

            exit(0);
        }
    }
    return 1;
}

void piping(char *token[], long long int num_arg)
{
    char pipe_command[10000] = "";
    no_of_pipe_cmds = 0;

    for (int i = 0; i < num_arg; i++)
    {
        strcat(pipe_command, token[i]);
        strcat(pipe_command, " ");
    }

    pipe_commands[0] = strtok_r(pipe_command, "|\n", &inputStrPtr);

    while (pipe_commands[no_of_pipe_cmds] != NULL)
    {
        no_of_pipe_cmds++;
        pipe_commands[no_of_pipe_cmds] = strtok_r(NULL, "|\n", &inputStrPtr);
    }

    int check = piping_child_process();
    if (check == 0)
        return;
}