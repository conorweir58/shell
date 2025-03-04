#include "shell_commands.h"

//
//  Command functions
//

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
    // Error handling for getcwd
    if(cwd == NULL)
    {
        printf("Error: Could not get the Current Working Directory\n");
        exit(1);
    }

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
    getcwd(cwd, sizeof(cwd)); // Get current working directory
    // Error handling for getcwd
    if(cwd == NULL)
    {
        printf("Error: Could not get the Current Working Directory\n");
        exit(1);
    }

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