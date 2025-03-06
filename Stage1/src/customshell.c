#include "customshell.h"
#include "shell_commands.h"

int main(int argc, char *argv[]) {
    
    // Variables for input processing
    char buf[MAX_BUFFER];  // Input buffer
    char *args[MAX_ARGS];  // Array of argument strings
    char **arg;            // Pointer for iterating through args
    char file[MAX_PATH];   // Batch File path buffer
    char cwd[MAX_PATH];    // Current working directory 
    
    getcwd(cwd, sizeof(cwd)); // Get current working directory
    // Error handling for getcwd
    if(cwd == NULL)
    {
        printf("Error: Could not get the Current Working Directory\n");
        return 1;
    }

    // COULD THIS BE A FUNCTION??
    char shell_path[MAX_PATH]; // Create variable to store the path of the shell
    strcpy(shell_path, cwd); // Copy the current working directory to the shell path
    strcat(shell_path, "/customshell"); // Append the name of the shell to the shell path to get the full SHELL enviorment variable path
    
    if(setenv("SHELL", shell_path, 1) != 0) // Set the environment variable SHELL to the path of the shell
    {
        // Error handling for setenv -> if setenv returns a non-zero value, print error message and exit
        printf("Error: Could not set the SHELL environment variable\n");
        return 1;
    }

    //
    // set_shell_path(); // Set the SHELL environment variable to the path of the shell
    //

    // COULD THIS IF STATEMENT BE A FUNCTION??
    // Check if a batch file was passed as an argument
    if(!argv[1])
    {
        // if no batch file, display startup message
        start_up_shell();
    }
    else
    {
        // if batch file - call batch file function and pass it the batch file
        strcpy(file, argv[1]);
        execute_batch_file(file);
        // execute the batch file and exits the shell to prevent further input
    }

    char *prompt = get_prompt();

    // Main input loop
    while (!feof(stdin)) { 

        // Display prompt
        printf("\033[0;32m"); // Set text colour to Green
        printf("<%s>$ ", prompt); // Print prompt in yellow
        printf("\033[0m"); // Reset text colour
        fflush(stdout); // Ensure output is printed immediately

        // Read a line of input
        if (fgets(buf, MAX_BUFFER, stdin)) { 

            // Tokenize input into args array
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)));

            // If input is not empty, process command
            if (args[0]) {
                // call execute command function which will process the command
                execute_command(args);
            }
        }
    }
    return 0;
}