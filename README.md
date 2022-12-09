## OS Assignment 2
#### Poorva Pisal
**2020113001**
<br>

## Compiling and Running the Shell

run `make` to generate the executable

run `./a.out` to run the shell

Use the command `exit` to exit the shell

## Commands

The shell takes the commands listed below. Commands can be seperated by ';' or by the newline character '\n' and background processes are indicated using an '&' character.

#### cd command
Implemented in **`cd.c`**

- `cd <path>` - changes directory to  path
- `cd` or `cd .` - changes directory to current directory
- `cd ..` - changes directory to the parent directory
- `cd ~` - changes directory to the shell's root directory (i.e the directory the shell is executing from)
- `cd -` - changes directory to the previous directory (i.e the last directory that the shell prompt showed)

#### echo command
Implemented in **`echo.c`**

- `echo` - prints empty line
- `echo <text>` - prints text
- `echo "text     'text2'       "`  - prints:  "text 'text2'"  (i.e handles tabs and spaces)

#### pwd command
Implemented in **`pwd.c`**

- `pwd` - prints absolute path of current directory

#### ls command

Implemented in **`ls.c`**

- ls takes arguments:  `<flags>` and `<path>` 
- It lists the files of the path mentioned (directory or file). 
- `ls` - if no path is mentioned, lists the files in the current directory
- `ls -l` - displays the files in the long listing format
- `ls -a` - displays all files including hidden files
- `ls -al/-la/-a -l/-l -a` - long listing format of all files including hidden files
- `ls -l <dir1> -a <dir2>` - for any variation of flags and multiple directories, the flags will be applied to all directories (i.e order doesn't matter).
- Specific colour coding has been done to differentiate between file names, directories and executables in the output. [eg: white for files and blue for directories etc.]

#### Foreground processes
Implemented in **`foregroundprocess.c`**

- The shell waits till the current process finishes executing and displays the time taken in the next prompt.
- 

#### Background processes
Implemented in **`backgroundprocess.c`**

- `<command> &`  - Using '&' at the end a command invoke a background process.
- The process will run in the background and display a message when it exits.

#### pinfo command
Implemented in **`pinfo.c`**

- `pinfo` - prints process information of the shell
- `pinfo <pid>` - prints process information of the given pid

#### discover command
Implemented in **`main.c`**

- The command will have the following optional command line arguments: 
`discover <target_dir> <type_flags> "<file_name>"`
- `<target_dir>`: target directory specified using ‘.’, ‘..’, ‘~’ or directory_name or directory_path.
- flag `-d`: searches for all directories
- flaf `-f`: searches for all files
- `"<file_name>"`: name of the file/directory to be found in the given directory hierarchy.
- If neither the file type nor the file name is provided as a command line argument, then all the contents of the target directory are printed.
- Does not check for hidden files

#### history command
Implemented in **`history.c`**

- Outputs the list of previously used commands.
- The default number of commands outputted is 10. 
- The default number of commands stored is 20.
- The older commands are overwritten if more than the set number of commands are entered.
- A command is not stored in history if it is the same as the previously entered command.

#### Input/Output Redirection 
Implemented in **`redirection_piping.c`**

- Using the symbols <, > and >>, the output of commands, usually written to stdout, can be redirected to another file,
or the input taken from a file other than stdin.
- Multiple inputs and outputs have not been handled.

#### Command Pipelines
Implemented in **`redirection_piping.c`**

- A pipe, identified by |, redirects the output of the command on the left as input to the command on the right. One or more commands can be piped as the following examples show. 
- Input/output redirection can also occur within command pipeline.

#### User defined commands
Implemented in **`redirection_piping.c`** and **`sig.c`**

- **jobs** - This command prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, long with their job number (a sequential number assigned by your shell), process ID, and their state, which can either be running or stopped. There may be flags specified as well. If the flag specified is -r, then print only the running processes else if the flag is -s then print the stopped processes only.
- **sig** - Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to the signal number to that process. The shell should throw an error if no job with the given number exists. 
- **fg** - Brings the running or stopped background job corresponding to job number to the foreground, and changes its state to running. The shell should throw an error if no job with the given job number exists.
- **bg** - Changes the state of a stopped background job to running (in the background). The shell should throw an error if no background job corresponding to the given job number exists, and do nothing if the job is already running in the background.

#### Signal handling
Implemented in **`signal.c`** and **`main.c`**

- **CTRL-Z** It should push any currently running foreground job into the background, and change its state from ‘running’ to ‘stopped.’ This should have no effect on the shell if there is no foreground process running.
- **CTRL-C** It should interrupt any currently running foreground job, by sending it the SIGINT signal. This should have no effect on the shell if there is no foreground process running.
- **CTRL-D** It should log you out of your shell, without having any effect on the actual terminal.

#### Autocompletion 
Implemented in **`main.c`**

- The tab key can be pressed at any time of your prompt. When you press the tab key, it will print all files which have the same starting letters as the ls command and then again show the same prompt. If there is only one file/folder which has the same starting characters, the prompt should be filled with that. If it's a file, a space should be placed after the file. If it’s a folder, a ‘/’ should be put after the folder name.

#### exit command
- `exit` - exits the shell