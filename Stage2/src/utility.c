/*
STUDENT NAME: CONOR WEIR
STUDENT ID: 23418374
ACADEMIC INTEGRITY STATEMENT: I acknowledge DCU's academic integrity policy.
*/

#include "customshell.h"
#include "shell_commands.h"

//
//  Helper functions
// 

void start_up_shell()
{
    // variables for storing the user name
    char user[MAX_BUFFER];  // User name buffer
    char* pTmp; // Temporary pointer for storing the user name

    // Get the user name from the USER environment variable and store it in the user variable
    if((pTmp = getenv("USER")))
    {
        // if it works -> copy the user name to the user variable
        strcpy(user, pTmp);

        if(user == NULL)
        {
            fprintf(stderr, "Error: Could not retrieve USER variable.\n"); // Print error message to stderr -> don't use perror as no relevant error message possible with it
            strcpy(user, "User"); // Set the user name to "User" as a default
        }
    }
    else
    {
        // if it doesn't work -> print error message and set the user name to "User"
        fprintf(stderr, "Could not retrieve USER variable.\n"); // Print error message to stderr
        strcpy(user, "User"); // Set the user name to "User" as a default
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

void set_shell_path()
{
    char cwd[MAX_PATH];    // Current working directory 
    
    if(getcwd(cwd, sizeof(cwd)) == NULL) // Get current working directory
    {
        perror("Could not retrieve the Current Working Directory"); // Print error message along with the error message from the system
        exit(1); // Exit the shell 
    }

    char shell_path[MAX_PATH]; // Create variable to store the path of the shell
    strcpy(shell_path, cwd); // Copy the current working directory to the shell path
    strcat(shell_path, "/customshell"); // Append the name of the shell to the shell path to get the full SHELL enviorment variable path
    
    if(setenv("SHELL", shell_path, 1) != 0) // Set the environment variable SHELL to the path of the shell
    {
        // Error handling for setenv -> if setenv returns a non-zero value, print error message and exit
        fprintf(stderr, "Error: Could not set the SHELL environment variable\n"); // Print error message to stderr
        exit(1); // Exit the shell
    }
}

// Function to get the shell prompt based on the current working directory	
char *get_prompt()
{
    static char cwd[MAX_PATH];    // Current working directory -> static so it doesn't get destroyed after the function ends
    if(getcwd(cwd, sizeof(cwd)) == NULL) // Get current working directory
    {
        perror("Could not retrieve the Current Working Directory"); // Print error message along with the error message from the system
        return "CustomShell"; // Return a Default prompt if the current working directory cannot be retrieved
    }

    char *prompt = cwd;  // Shell prompt
    return prompt; // Return the shell prompt to be printed in main.c
}

void execute_batch_file(char *file)
{
    FILE *pfile = NULL; // Initialize file pointer
    pfile = fopen(file, "r"); // Open the batch file in read mode using the file path

    if (pfile) // If the file exists and was opened successfully process the batch file
    {
        char buf[MAX_BUFFER];  // Input buffer
        char *args[MAX_ARGS];  // Array of argument strings
        char **arg;            // Pointer for iterating through args

        // Loop through a line of input
        while (fgets(buf, MAX_BUFFER, pfile))
        {            
            // Tokenize input into args array
            arg = args; // Set arg to the start of the args array
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS))); // Loop through the input and tokenize it into the args array
            
            // If input is not empty, process command
            if (args[0])
            {
                execute_command(args); // call execute command function which will process the command
            }
        }
        fclose(pfile); // Close the batch file
        execute_quit(); // Exits the shell after the batch file is processed
    }
    else
    {
        perror("Could not open batch file!"); // Print error message along with the error message from the system
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
            fprintf(stderr, "Error: Too many arguments for command \"cd\"!\n"); // Print error message to stderr
            return; // Return to the main loop
        }

        // set path to the first argument after cd, if there is none, the path becomes NULL
        char *path = args[1];
        execute_cd(path); // Call the execute_cd function with the path

        // get_prompt(); // Update the prompt after changing directory
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
        execute_external_command(args);
    }



    // else
    // {
    //     command_not_found(args[0]); // Print error message if command is not found
    // }
}
