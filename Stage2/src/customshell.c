/*
STUDENT NAME: CONOR WEIR
STUDENT ID: 23418374
ACADEMIC INTEGRITY STATEMENT: I acknowledge DCU's academic integrity policy.
*/

#include "customshell.h"
#include "shell_commands.h"

int main(int argc, char *argv[]) {
    
    // Variables for input processing
    char buf[MAX_BUFFER];  // Input buffer
    char *args[MAX_ARGS];  // Array of argument strings
    char **arg;            // Pointer for iterating through args
    // char file[MAX_PATH];   // Batch File path buffer

    set_shell_path(); // Set the SHELL environment variable to the path of the shell executable

    check_batch(argv); // Check if a batch file was passed as an argument and launch shell correspondingly

    // Main input loop -> Loops until quit or EOF
    while (!feof(stdin)) { 

        char *prompt = get_prompt(); // Get the shell prompt using get_prompt function (in utility.c)

        // Display prompt
        printf("\033[0;32m"); // Set text colour to Green
        printf("<%s>$ ", prompt); // Print prompt in yellow
        printf("\033[0m"); // Reset text colour
        fflush(stdout); // Ensure output is printed immediately

        // Read a line of input
        if (fgets(buf, MAX_BUFFER, stdin)) { 

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
    }

    return 0;
}