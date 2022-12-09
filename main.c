#include "global.h"

int exit_status = 0;
char inputString[MAX_INPUT_CMD_LEN];

char *history[20];
int hist_count = 0;

clock_t t;
// hell //hello
// hist.c hist.txt
int comp(char a[], char b[])
{
    int flag = 0, i = 0;                 // integer variables declaration
    while (a[i] != '\0' && b[i] != '\0') // while loop
    {
        if (a[i] != b[i])
        {
            flag = 1;
            break;
        }
        else if (a[i + 1] == '\0' || b[i + 1] == '\0')
        {
            flag = 1;
            i++;
            break;
        }

        i++;
    }
    if (flag == 0)
        return 0;
    else
        return i;
}

bool StartsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
    {
        return 1;
    }
    return 0;
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void add_to_history(char *command)
{
    // printf("%d\n", strcmp(command, history[hist_count - 1]));
    if (strcmp(command, history[hist_count - 1]) != 0)
    {
        if (hist_count < 20)
        {
            history[hist_count] = strdup(command);
            // printf("--- %s --- %s\n", history[hist_count], command);
            hist_count++;
        }

        else
        {
            for (int i = 1; i < 20; i++)
                history[i - 1] = history[i];
            history[19] = strdup(command);
        }
    }
}

void get_input(char *input)
{
    char input_line[MAX_INPUT_CMD_LEN];
    char *token[MAX_INPUT_CMD_LEN];
    char *commands[MAX_INPUT_CMD_LEN];
    long long int i = 0;
    char *input_ptr;
    char *cmd_ptr;

    strcpy(input_line, input);

    commands[0] = strtok_r(input, ";\n", &input_ptr);

    while (commands[i])
    {
        i++;
        commands[i] = strtok_r(NULL, ";\n", &input_ptr);
    }

    for (long long int j = 0; j < i; j++)
    {
        token[0] = strtok_r(commands[j], " \t", &cmd_ptr);
        long long int num_arg = 0;

        while (token[num_arg])
        {
            num_arg++;
            token[num_arg] = strtok_r(NULL, " \t", &cmd_ptr);
        }

        execute(input_line, token, num_arg);
    }
}

void execute(char *input, char *tokens[], long long int num_arg)
{
    long long int redirection_position;
    int redirection_flag = redirection_check(tokens, num_arg, &redirection_position);
    int piping_flag = piping_check(tokens, num_arg);

    // printf("input: %s\n", input);

    if (redirection_flag > 0 && piping_flag == 0)
    {
        redirection(tokens, num_arg, redirection_flag, redirection_position);
        return;
    }

    if (piping_flag == 1)
    {
        piping(tokens, num_arg);
        return;
    }

    else if (strcmp(tokens[0], "cd") == 0)
    {
        fore = 0;
        cd(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "pwd") == 0)
    {
        fore = 0;
        pwd();
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "ls") == 0)
    {
        fore = 0;
        ls(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "jobs") == 0)
    {
        fore = 0;
        jobs(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "echo") == 0)
    {
        fore = 0;
        echo(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "history") == 0)
    {
        fore = 0;
        add_to_history(tokens[0]);
        print_history(history, hist_count);
    }

    else if (strcmp(tokens[num_arg - 1], "&") == 0)
    {
        fore = 0;
        char *command[100];

        for (long long int j = 0; j < num_arg - 1; j++)
            command[j] = tokens[j];

        background(command);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "pinfo") == 0)
    {
        fore = 0;
        pinfo(tokens[1], num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "sig") == 0)
    {
        fore = 0;
        sig(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "fg") == 0)
    {
        fore = 1;
        fg(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "bg") == 0)
    {
        fore = 0;
        bg(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "discover") == 0)
    {
        fore = 0;
        discover(tokens, num_arg);
        add_to_history(tokens[0]);
    }

    else if (strcmp(tokens[0], "exit") == 0)
    {
        fore = 0;
        add_to_history(tokens[0]);
        chdir(root_dir);
        save_history(history, hist_count);
        exit_status = 1;
    }

    else
    {
        secs1 = time(NULL);
        foreground(tokens);
        secs2 = time(NULL);
        if (secs2 - secs1 >= 1)
            fore = 1;
        add_to_history(tokens[0]);
    }
}

int main()
{
    fore = 0;
    num_of_proc = 0;
    total_jobs = 0;
    getcwd(root_dir, sizeof(root_dir));

    char *inp = malloc(sizeof(char) * MAX_INPUT_CMD_LEN);
    char c;

    FILE *fp = fopen("history.txt", "a+");

    fprintf(fp, "\n");
    fseek(fp, 0, 0);
    char buf[100];

    while (fgets(buf, sizeof(buf), fp))
    {
        if (strcmp(buf, "\0") != 0 && strcmp(buf, " ") != 0 && strcmp(buf, "\n") != 0)
        {
            history[hist_count] = strdup(buf);
            // printf("%d:%s", hist_count, history[hist_count]);
            history[hist_count][strlen(history[hist_count]) - 1] = '\0';
            hist_count++;
        }
    }

    fclose(fp);

    signal(SIGTSTP, ctrl_z);
    signal(SIGINT, ctrl_c);

    char *input;

    // printf("root: %s\n", root_dir);

    while (!exit_status)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        t = clock();
        printf("\n");
        prompt();
        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

        // printf("foreground process took %f seconds to execute \n", time_taken);
        // input = fgets(inputString, MAX_INPUT_CMD_LEN, stdin);

        // if (input == NULL)
        // {
        //     printf("\n");
        //     exit_status = 1;
        //     continue;
        // }

        // get_input(inputString);
        // exit_status = 1;

        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                      // printf("\rarrow key: %s", buf);
                    }
                }
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 9)
                {
                    int count = 0;
                    char *word = "";
                    char *word1 = "";
                    char *last = strrchr(inp, ' ') + 1;
                    // TAB character
                    struct dirent *de1; // Pointer for directory entry

                    // opendir() returns a pointer of DIR type.
                    DIR *dr1 = opendir(".");

                    if (dr1 == NULL) // opendir returns NULL if couldn't open directory
                    {
                        printf("Could not open current directory");
                        return 0;
                    }

                    while ((de1 = readdir(dr1)) != NULL)
                    {
                        if (StartsWith(de1->d_name, last))
                        {
                            count++;
                            // printf("\n%s", de1->d_name);
                            word = de1->d_name + strlen(last);
                            if (is_directory(de1->d_name))
                                strcat(word, "/");
                            else
                                strcat(word, " ");
                        }
                    }

                    closedir(dr1);

                    if (count > 1)
                    {
                        struct dirent *de;
                        DIR *dr = opendir(".");

                        if (dr == NULL) // opendir returns NULL if couldn't open directory
                        {
                            printf("Could not open current directory");
                            return 0;
                        }

                        count = 0;
                        char *compare = "";
                        int k1 = 0;

                        while ((de = readdir(dr)) != NULL)
                        {
                            if (StartsWith(de->d_name, last))
                            {
                                if (count == 0)
                                {
                                    compare = de->d_name;
                                }
                                else
                                {
                                    k1 = comp(compare, de->d_name);
                                    // compare = de->d_name - (de->d_name + k1);
                                    strncpy(compare, &de->d_name[0], k1);
                                    compare[k1] = '\0';
                                }
                                count++;
                                if (is_directory(de->d_name))
                                    printf("\n%s/", de->d_name);
                                else
                                    printf("\n%s", de->d_name);

                                word1 = compare + strlen(last);
                            }
                        }
                        closedir(dr);

                        printf("\n");
                        // prompt();
                        
                        for (int i = 0; i < strlen(word1); i++)
                        { // TABS should be 8 spaces
                            inp[pt++] = word1[i];
                        }

                        prompt();
                        printf("%s", inp);
                        //fflush(stdout);
                    }

                    else
                    {
                        // inp = inp - last;
                        printf("%s", word);
                        for (int i = 0; i < strlen(word); i++)
                        { // TABS should be 8 spaces
                            inp[pt++] = word[i];
                        }
                    }

                    // inp[pt++] = word;
                }

                else if (c == 4)
                {
                    exit(0);
                }
                else
                {
                    printf("%d\n", c);
                }
            }
            else
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        if (inp == NULL)
        {
            printf("\n");
            exit_status = 1;
            continue;
        }

        // printf("\nInput Read: [%s]\n", inp);
        printf("\n");
        get_input(inp);
    }

    return 0;
}