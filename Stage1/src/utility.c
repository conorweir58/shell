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

void execute_ls(char *path)
{
    if (path != NULL)
    {
        // char *ls_command = "ls -al "; // Create the ls -al command
        char *command = malloc(strlen("ls -al ") + strlen(path) + 1); // Allocate memory for the command (enough for the command and the path)

        strcpy(command, "ls -al "); // Copy the ls -al command to the command
        strcat(command, path); // Append the path to the command
        system(command); // Execute the command
        free(command); // Free the malloc'd memory
        return;
    }
    else
    {
        system("ls -al"); // Lists all files and directories in the current directory
    }
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

void execute_echo(char **args)
{
    for (int i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
}

void execute_pause()
{
    // prompts the user to press enter to continue
    printf("Press Enter to continue...\n");

    // waits for the user to press enter -> getchar() reads the next character from the standard input so user will need to hit enter to proceed no matter what
    getchar();
}

void execute_help()
{
    FILE *pfile = NULL;
    pfile = fopen("../manual/readme", "r");

    if (pfile)
    {
        char c;
        while ((c = getc(pfile)) != EOF)
        {
            putchar(c);
        }
        fclose(pfile);
        putchar('\n');
    }
    else
    {
        printf("Error: Could not find help manual!\n");
    }

}

void command_not_found(char *arg)
{
    printf("%s: Command not found\n", arg);
}

// CURRENT ERROR:


// LEFT TO ADD:
// UPDATE LS COMMAND TO HAVE A PATH ARGUMENT -> DONE
// ECHO COMMAND -> DONE
// PAUSE COMMAND -> DONE
// HELP COMMAND 
// UPDATE THE SHELL=/CUSTOMSHELL
// BATCH FILE SUPPORT
// CHECK TO SEE IF FORK AND EXEC CAN BE USED -> might not be till stage 2 for external commands -> ask graham
// check is better to just use system() for the commands for clear etc or to use fork and exec
// create the readme
// create the makefile

// ADDITIONAL NOTES:
// possibly change the execute_cd and execute_ls functions to take in a char **args variable
// check error handling for all functions -> using fopen, freopen, etc you should check the return status and indicate if an error has occurred, and take appropriate action.
// check if possible to make sure pause doesn't try to run a command 