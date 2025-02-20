#include "customshell.h"


// Function to get the shell prompt based on the current working directory	
char *get_prompt()
{
    char cwd[MAX_PATH];    // Current working directory
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