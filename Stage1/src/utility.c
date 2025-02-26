#include "customshell.h"

// Function to get the shell prompt based on the current working directory	
char *get_prompt()
{
    static char cwd[MAX_PATH];    // Current working directory -> static so it doesn't get destroyed after the function ends
    getcwd(cwd, sizeof(cwd)); // Get current working directory
    char *prompt = cwd;  // Shell prompt
    return prompt;
}

void execute_batch_file(char *file)
{
    FILE *pfile = NULL;
    pfile = fopen(file, "r");

    if (pfile)
    {
        char buf[MAX_BUFFER];  // Input buffer
        char *args[MAX_ARGS];  // Array of argument strings
        char **arg;            // Pointer for iterating through args

        // Loop through a line of input
        while (fgets(buf, MAX_BUFFER, pfile))
        {
            // buf[strcspn(buf, "\n")] = '\0'; // Remove newline character from input
            
            // Tokenize input into args array
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)));
            
            // If input is not empty, process command
            if (args[0])
            {
                // List of commands
                if (!strcmp(args[0], "clr"))
                {
                    execute_clear();
                }
                else if (!strcmp(args[0], "quit"))
                {
                    execute_quit();
                }
                else if (!strcmp(args[0], "dir"))
                {
                    char *path = args[1];

                    execute_ls(path);
                }
                else if (!strcmp(args[0], "environ"))
                {
                    execute_environ();
                }
                else if (!strcmp(args[0], "cd"))
                {
                    // if there is more than one path after cd, print an error message -> checking for both args[1] and args[2] makes sure cd on its own still works
                    if(args[1] && args[2])
                    {
                        printf("Error: Too many arguments for command \"cd\"!\n");
                        continue;
                    }

                    // set path to the first argument after cd, if there is none, the path becomes NULL
                    char *path = args[1];
                    execute_cd(path);
                }
                else if (!strcmp(args[0], "echo"))
                {
                    execute_echo(args);
                }
                else if (!strcmp(args[0], "pause"))
                {
                    execute_pause();
                }
                else if (!strcmp(args[0], "help"))
                {
                    execute_help();
                }
                else
                {
                    command_not_found(args[0]);
                }
            }
        }
        fclose(pfile);
        execute_quit();
    }
    else
    {
        printf("Error: Could not open batch file!\n");
        exit(1); // Exit the shell -> possibly change to return an error code
    }
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
// check if get_prompt should use a static variable and if so should i not init it every time its called
// add an error message function that takes in an integer based on the error and prints the appropriate error message