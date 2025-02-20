#include "customshell.h"


// Function to get the shell prompt based on the current working directory	
char *get_prompt()
{
    static char cwd[MAX_PATH];    // Current working directory -> static so it doesn't get destroyed after the function ends
    getcwd(cwd, sizeof(cwd)); // Get current working directory
    char *prompt = cwd;  // Shell prompt
    return prompt;
}

// Function to execute the "clear" command
void execute_clear() {
    system("clear"); // Clears the terminal screen
}

// Function to execute the "quit" command
void execute_quit() {
    exit(0); // Exits the shell
}

void execute_ls()
{
    system("ls -al"); // Lists all files and directories in the current directory
}

void execute_cd(char *path)
{
    char cwd[MAX_PATH];    // Current working directory
    getcwd(cwd, sizeof(cwd)); // Get current working directory

    // if there is no path, print current working directory
    if (path == NULL)
    {
        printf("%s\n", cwd);
        return;
    }

    // change the directory to the path, if change directory fails (equals -1), prints error message
    if (chdir(path) == -1)
    {
        printf("cd: %s: No such file or directory\n", path);
        return;
    }

    // set the environment variable OLDPWD to the old CWD
    setenv("OLDPWD", cwd, 1);

    // update cwd to the new Current Working Directory after chdir(path)
    getcwd(cwd, sizeof(cwd));

    // set the environment variable PWD to the new path of the CWD
    setenv("PWD", cwd, 1);
}

// Function to execute the "environ" command -> lists all environment variables/strings
void execute_environ()
{
    char **env = environ;
    while (*env) 
    {
        printf("%s\n", *env++);
    }
}

void command_not_found(char *arg)
{
    printf("%s: Command not found\n", arg);
}