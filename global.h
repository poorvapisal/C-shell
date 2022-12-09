#ifndef GLOBAL_HEADERS_FUNCS
#define GLOBAL_HEADERS_FUNCS

#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pwd.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

#define MAX_INPUT_CMD_LEN 100000
#define MAX_DIR_PATH_LEN 100000

char root_dir[MAX_DIR_PATH_LEN];
char current_dir[MAX_DIR_PATH_LEN];
char prev_directory[MAX_DIR_PATH_LEN];
time_t secs1, secs2;
int fore;
int shell_pid;

struct process
{
    char name[100];
    pid_t pid;
    int status;
    int job_num;
};

struct process fg_process;
struct process proc[100];
int num_of_proc;
int total_jobs;

char hostname[_SC_HOST_NAME_MAX];
char username[_SC_LOGIN_NAME_MAX];

void prompt();
void get_input(char *input);
void execute(char *input, char *command[], long long int arg_count);
void cd(char *command[], long long int arg_count);
void echo(char *command[], long long int arg_count);
void pinfo(char command[], long long int arg_count);
void ls(char *command[], long long int arg_count);
void discover(char *command[], long long int arg_count);
void sig(char *command[], long long int arg_count);
int piping_check(char *token[], long long int num_arg);
void piping(char *token[], long long int num_arg);
int redirection_check(char *token[], long long int num_arg, long long int *position);
void redirection(char *token[], long long int arg_count, int redirection_flag, long long int redir_position);
void fg(char *command[], long long int arg_count);
void bg(char *command[], long long int arg_count);
void foreground(char *command[]);
void background(char *command[]);
void delete_process(int pid);
void pwd();
void jobs(char *command[], long long int arg_count);
void save_history(char *history[], int hist_count);
void print_history(char *history[], int hist_count);
int is_file(char *path);
int is_directory(char *path);
void ctrl_c(int sig_num);
void ctrl_z(int sig_num);

void red();
void yellow();
void blue();
void green();
void reset();

#endif
