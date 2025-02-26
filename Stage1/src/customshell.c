#include "customshell.h"

int main(int argc, char *argv[]) {
    
    // Variables for input processing
    char buf[MAX_BUFFER];  // Input buffer
    char *args[MAX_ARGS];  // Array of argument strings
    char **arg;            // Pointer for iterating through args
    char file[MAX_PATH];   // Batch File path buffer

    char *prompt = get_prompt();

    // Main input loop
    while (!feof(stdin)) { 

        // Check for batch file input
        if(argv[1])
        {
            // if batch file - call batch file function and pass it the batch file
            strcpy(file, argv[1]);
            execute_batch_file(file);
            // execute the batch file and exits the shell to prevent further input
        }

        // Display prompt
        printf("<%s>$ ", prompt);
        fflush(stdout); // Ensure output is printed immediately

        // Read a line of input
        if (fgets(buf, MAX_BUFFER, stdin)) { 

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

                    // execute the cd command with the path
                    execute_cd(path);

                    // Update the prompt to the new cwd
                    prompt = get_prompt();
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
    }
    return 0;
}