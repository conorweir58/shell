#include "customshell.h"
#include "shell_commands.h"

//
//  Helper functions
// 

void start_up_shell()
{
    // variables for storing the user name
    char user[MAX_BUFFER];  // User name buffer
    char* pTmp;

    // Get the user name from the USER environment variable
    if((pTmp = getenv("USER")))
    {
        // if it works -> copy the user name to the user variable
        strcpy(user, pTmp);
    }
    else
    {
        // if it doesn't work -> print error message and set the user name to "User"
        fprintf( stderr, "Could not retrieve USER variable.\n");
        strcpy(user, "User");
    }

    // Print the welcome message box with the shell information -> text is in Cyan
    printf("###############################################################################################\n");
    printf("##                                                                                           ##\n");
    printf("##                                  \033[0;36mWelcome to CustomShell!\033[0m                                  ##\n");
    printf("##                                \033[0;36mAuthor: Conor Weir (23418374)\033[0m                              ##\n");
    printf("##                                                                                           ##\n");
    printf("##                            \033[0;36mType \"help\" for a list of commands.\033[0m                            ##\n");
    printf("##                                                                                           ##\n");
    printf("##                            \033[0;36mType \"quit\" to exit the shell.\033[0m                                 ##\n");
    printf("##                                                                                           ##\n");
    printf("###############################################################################################\n");
    printf("\n"); // New line to space out the user message from the welcome message
    printf("\033[0;34mWelcome %s!\033[0m \n", user); // Print user message in Blue
    printf("\n"); // New line to space out the prompt from the welcome message
}

// void set_shell_path()
// {
//     char cwd[MAX_PATH];    // Current working directory 
    
//     getcwd(cwd, sizeof(cwd)); // Get current working directory
//     // Error handling for getcwd
//     if(cwd == NULL)
//     {
//         printf("Error: Could not get the Current Working Directory\n");
//         exit(1);
//     }

//     // COULD THIS BE A FUNCTION??
//     char shell_path[MAX_PATH]; // Create variable to store the path of the shell
//     strcpy(shell_path, cwd); // Copy the current working directory to the shell path
//     strcat(shell_path, "/customshell"); // Append the name of the shell to the shell path to get the full SHELL enviorment variable path
    
//     if(setenv("SHELL", shell_path, 1) != 0) // Set the environment variable SHELL to the path of the shell
//     {
//         // Error handling for setenv -> if setenv returns a non-zero value, print error message and exit
//         printf("Error: Could not set the SHELL environment variable\n");
//         exit(1);
//     }
// }

// Function to get the shell prompt based on the current working directory	
char *get_prompt()
{
    static char cwd[MAX_PATH];    // Current working directory -> static so it doesn't get destroyed after the function ends
    getcwd(cwd, sizeof(cwd)); // Get current working directory

    // Error handling for getcwd
    if(cwd == NULL)
    {
        printf("Error: Could not get the Current Working Directory\n");
        exit(1);
    }

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
            // Tokenize input into args array
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)));
            
            // If input is not empty, process command
            if (args[0])
            {
                // call execute command function which will process the command
                execute_command(args);
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

void execute_command(char **args)
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
            return;
        }

        // set path to the first argument after cd, if there is none, the path becomes NULL
        char *path = args[1];
        execute_cd(path);

        get_prompt();
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


// CURRENT ERROR:
// NONE

// REMOVE:
// set_shell_path(); FUNCTION POSSIBLY -> COME BACK TO IT

// LEFT TO ADD:
// UPDATE LS COMMAND TO HAVE A PATH ARGUMENT -> DONE
// ECHO COMMAND -> DONE
// PAUSE COMMAND -> DONE
// HELP COMMAND -> DONE
// UPDATE THE SHELL=/CUSTOMSHELL
// BATCH FILE SUPPORT -> DONE
// CREATE A COMMANDS.C FILE CONTAINING ALL COMMAND FUNCTIONS -> DONE
// PUT THE SETENV SHELL INTO A FUNCTION
// ADD ERROR HANDLING FOR BATCH FILE
// CHECK TO SEE IF FORK AND EXEC CAN BE USED -> might not be till stage 2 for external commands -> ask graham
// check is better to just use system() for the commands for clear etc or to use fork and exec
// create the readme !!! -> DONE
// create the makefile -> DONE

// ADDITIONAL NOTES:
// possibly change the execute_cd and execute_ls functions to take in a char **args variable
// check error handling for all functions -> using fopen, freopen, etc you should check the return status and indicate if an error has occurred, and take appropriate action.
// check if possible to make sure pause doesn't try to run a command
// check if get_prompt should use a static variable and if so should i not init it every time its called
// add an error message function that takes in an integer based on the error and prints the appropriate error message
// add a description to each file