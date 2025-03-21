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
            if(args[0])
            {
                execute_command(args); // Process the command
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

int check_internal(char **args)
{
    // List of internal commands
    char *internal_commands[] = 
    {
        "clr",
        "quit",
        "dir",
        "environ",
        "cd",
        "echo",
        "pause",
        "help"
    };

    for (int i = 0; i < sizeof(internal_commands)/sizeof(internal_commands[0]); i++) // Loop through the internal commands array
    {
        if (!strcmp(args[0], internal_commands[i]))
        {
            return 1; // Return 1 if the command is an internal command
        }
    }

    return 0; // Return 0 if the command is an external command
}

void execute_command(char **args)
{
    if(check_internal(args))
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
            io_redirection(args);

            char *path = args[1];

            execute_ls(path);

            freopen("/dev/tty", "w", stdout);
            freopen("/dev/tty", "r", stdin);
        }
        else if (!strcmp(args[0], "environ"))
        {
            io_redirection(args); // Process I/O redirection for the environ

            execute_environ(); // Call the execute_environ function

            freopen("/dev/tty", "w", stdout); // Reset stdout to the terminal
            freopen("/dev/tty", "r", stdin); // Reset stdin to the terminal
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
        }
        else if (!strcmp(args[0], "echo"))
        {
            io_redirection(args);

            execute_echo(args);

            freopen("/dev/tty", "w", stdout);
            freopen("/dev/tty", "r", stdin);
        }
        else if (!strcmp(args[0], "pause"))
        {
            execute_pause();
        }
        else if (!strcmp(args[0], "help"))
        {
            io_redirection(args);

            execute_help();

            freopen("/dev/tty", "w", stdout);
            freopen("/dev/tty", "r", stdin);
        }
    }
    else
    {
        execute_external_command(args);
    }
}

int check_background(char **args)
{
    if(check_internal(args)) // If the command is internal
    {
        return 0; // Return 0 -> command should not be run in background
    }

    int last_index = 0; // Int for storing position of the last argument in args
    while(args[last_index] != NULL) // Loop through the args array to find where it becomes NULL
    {
        last_index++; // Increment the last_index
    }
    last_index--; // Decrement the last_index to get the last valid argument

    if (last_index >= 0 && (!strcmp(args[last_index], "&"))) // Check that the last argument is "&" and prevent out of bounds access of last_index
    {
        return 1; // Return 1 if the command should be run in the background
    }

    return 0; // Return 0 if the command should not be run in the background
}

void io_redirection(char **args)
{
    // Initialize char and file pointers for input/output redirection
    char *pfile_in = NULL; // char pointer for input redirection file name
    char *pfile_out = NULL; // char pointer for output redirection file name

    // Initialize I/O flags
    int stdin_flag = 0; // Flag for input redirection
    int stdout_flag = 0; // Flag for output redirection
    int append_flag = 0; // Flag for appending to a file

    // Loop through the arguments to find I/O redirection symbols
    int i =0;
    while(args[i] != NULL)
    {
        if(!strcmp(args[i], "<")) // Check for input redirection symbol
        {
            stdin_flag = 1; // Set the input redirection flag
            args[i] = NULL; // Set the argument to NULL -> removes the redirection symbol

            if(args[i+1] == NULL) // Check if there is a file path after the "<"
            {
                fprintf(stderr, "Error: No output file provided for redirection\n"); // Print error message -> no relevant perror message possible with it
                exit(1); // Exit to prevent further command execution
            }

            i++; // Move to the next argument which should be the stdin file path
            
            pfile_in = args[i]; // Set the file pointer for input redirection to the provided file path
        }

        if(!strcmp(args[i], ">"))
        {
            stdout_flag = 1; // Set the output redirection flag
            args[i] = NULL; // Set the argument to NULL -> removes the redirection symbol

            if(args[i+1] == NULL) // Check if there is a file path after the ">"
            {
                fprintf(stderr, "Error: No output file provided for redirection\n"); // Print error message -> no relevant perror message possible with it
                exit(1); // Exit to prevent further command execution
            }

            i++; // Move to the next argument which should be the stdout file path

            pfile_out = args[i]; // Set the file pointer for output redirection to the provided file path
        }

        if(!strcmp(args[i], ">>"))
        {
            append_flag = 1; // Set the output redirection flag
            args[i] = NULL; // Set the argument to NULL -> removes the redirection symbol

            if(args[i+1] == NULL) // Check if there is a file path after the ">>"
            {
                fprintf(stderr, "Error: No output file provided for redirection\n"); // Print error message -> no relevant perror message possible with it
                exit(1); // Exit to prevent further command execution
            }

            i++; // Move to the next argument which should be the stdout file path

            pfile_out = args[i]; // Set the file pointer for output redirection to the provided file path
        }

        // Move to next argument
        i++;
    }

    if(stdin_flag) // If "<" found in command, i.e stdin_flag set to 1
    {
        if(access(pfile_in, F_OK) == -1) // Check if the file exists
        {
            perror("Error: Input file does not exist");
            exit(1); // Exit to prevent further command execution
        }

        if(access(pfile_in, R_OK) == -1) // Check if the file has read permissions
        {
            perror("Error: You lack permissions to read from this file");
            exit(1); // Exit to prevent further command execution
        }

        if(freopen(pfile_in, "r", stdin) == NULL) // Open the file for reading and set it as the standard input
        {
            perror("Error: Could not open Input file to read from");
            exit(1); // Exit to prevent further command execution
        }
    }

    if(stdout_flag) // If ">" found in command, i.e stdout_flag set to 1
    {
        if(access(pfile_out, F_OK) != -1) // Check if the file exists -> if it doesn't freopen will create it
        {
            if(access(pfile_out, W_OK) == -1) // If the file exists we check to see if user has permissions to write to it
            {
                perror("Error: You lack permissions to write to this file");
                exit(1); // Exit to prevent further command execution
            }
        }

        if(freopen(pfile_out, "w", stdout) == NULL) // Open the file for writing and set it as the standard output
        {
            perror("Error: Could not open Output file to write to");
            exit(1); // Exit to prevent further command execution
        }
    }
    else if(append_flag) // If ">>" found in command, i.e append_flag set to 1
    {
        if(access(pfile_out, F_OK) != -1) // Check if the file exists -> if it doesn't freopen will create it
        {
            if(access(pfile_out, W_OK) == -1) // If the file exists we check to see if user has permissions to write to it
            {
                perror("Error: You lack permissions to write to this file");
                exit(1); // Exit to prevent further command execution
            }
        }
        
        if(freopen(pfile_out, "a", stdout) == NULL) // Open the file for appending and set it as the standard output
        {
            perror("Error: Could not open Output file to append to");
            exit(1); // Exit to prevent further command execution
        }
    }
}