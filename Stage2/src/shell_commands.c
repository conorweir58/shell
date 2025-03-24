/*
STUDENT NAME: CONOR WEIR
STUDENT ID: 23418374
ACADEMIC INTEGRITY STATEMENT: I acknowledge DCU's academic integrity policy.
*/

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
    exit(0); // Exits the shell successfully
}

void execute_ls(char *path)
{
    if (path != NULL)
    {
        char *command = malloc(strlen("ls -al ") + strlen(path) + 1); // Allocate memory for the command (enough for the command and the path)

        if(command == NULL) // Check if malloc was successful
        {
            perror("Could not allocate memory for the command"); // Print error message along with the error message from the system
            free(command); // Free the malloc'd memory
            command = NULL; // Set the command to NULL
            return; // return back to main input loop
        }

        strcpy(command, "ls -al "); // Copy the ls -al command to the command
        strcat(command, path); // Append the path to the command
        system(command); // Execute the command

        free(command); // Free the malloc'd memory
        command = NULL; // Set the command to NULL
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
    // getcwd(cwd, sizeof(cwd)); // Get current working directory

    if(getcwd(cwd, sizeof(cwd)) == NULL) // Get current working directory
    {
        perror("Could not retrieve the Current Working Directory"); // Print error message along with the error message from the system
        exit(1); // exit the shell
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
        fprintf(stderr, "cd: %s: No such file or directory\n", path); // Print error message to stderr
        return;
    }

    // set the environment variable OLDPWD to the old CWD
    setenv("OLDPWD", cwd, 1);

    // update cwd to the new Current Working Directory after chdir(path)
    if(getcwd(cwd, sizeof(cwd)) == NULL) // Get current working directory
    {
        perror("Could not retrieve the Current Working Directory"); // Print error message along with the error message from the system
        exit(1); // exit the shell
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
    // For changing back to where the User is after displaying the manual
    char cwd[MAX_PATH];    // Current working directory

    if(getcwd(cwd, sizeof(cwd)) == NULL) // Get current working directory
    {
        perror("Could not retrieve the Current Working Directory"); // Print error message along with the error message from the system
        exit(1); // exit the shell
    }

    char *shell_path = getenv("SHELL"); // Stores path to the shell executable
    char shell_path_copy[MAX_PATH]; // Stores copy of the shell path

    if (shell_path != NULL) // Ensure the SHELL environment variable gotten successfully
    {
        
        strncpy(shell_path_copy, shell_path, sizeof(shell_path_copy) - 1); // Create a copy of the Shell Path -> ensures the original is not modified inside the environment variables
        shell_path_copy[sizeof(shell_path_copy) - 1] = '\0'; // Ensure null termination of the copied Shell Path

        char *last_slash = strrchr(shell_path_copy, '/'); // Find the last '/' character in the shell path
        if (last_slash && *(last_slash + 1) != '\0') // If the last '/' character is found and the next character is not null
        {
            *last_slash = '\0'; // Set the last '/' character to null to remove the executable name from the path
        }
        else
        {
            fprintf(stderr, "Error: Failed to extract last directory\n"); // Error handling for strrchr -> fprintf to stderr as no relevant error message possible with perror
            return; // Return to prevent further execution
        }
    }
    else
    {
        fprintf(stderr, "Error: Could not get the Shell Path\n"); // Error handling for getting SHELL environment variable -> fprintf to stderr as no relevant error message possible with perror
        return; // Return to prevent further execution
    }

    if (chdir(shell_path_copy) == -1) // Change directory to the shell path without the executable name -> ensures the manual is found
    {
        perror("Error changing directory to shell path"); 
        return;
    }

    system("more -d ./manual/readme"); // Display the manual using the more filter -> -d flag to allow for navigation prompt

    if (chdir(cwd) == -1) // Change directory back to the original working directory
    {
        perror("Error returning to previous directory");
    }
}

void execute_external_command(char **args)
{
    char *shell_path = getenv("SHELL"); // Stores path to the shell executable

    if (shell_path != NULL) // Ensure the SHELL environment variable gotten successfully
    {
        setenv("PARENT", shell_path, 1); // Set the PARENT environment variable to the path of the shell executable
    }
    else
    {
        fprintf(stderr, "Error: Could not get the Shell Path\n"); // Error handling for getting SHELL environment variable -> fprintf to stderr as no relevant error message possible with perror
        return; // Return to prevent further execution
    }

    pid_t pid; // Process ID
    int status; // Status of the child process
    int background_flag = 0; // Flag for checking if the command should be run as a background process

    background_flag = check_background(args); // Call check_background to see if the command must be executed in the background and update background flag 

    switch(pid = fork())
    {
        case -1: // Fork failed
            perror("Fork failed"); // Print error message along with the error message from the system
            exit(1); // Exit the shell
        case 0: // Child process

            if(background_flag) // If it is a command to be executed in background
            {
                int i = 0;
                while (args[i] != NULL) // Loop through args until we reach the end (NULL)
                {
                    if (strcmp(args[i], "&") == 0) // When we reach the "&" -> know it exists due to our check_background
                    {
                        args[i] = NULL; // Remove the "&" from the args
                        break;
                    }
                    i++; // increment through arguments
                }
            }

            io_redirection(args); // Process I/O redirection for the command -> don't revert file streams as execvp will close them or exit(1) will clean them up on error

            if (execvp(args[0], args) == -1) // Execute the command
            {
                fprintf(stderr, "Unable to execute command: %s\n", args[0]); // Error message for failed exec -> no relevant perror message so using fprintf
                exit(1); // Exit the child process if exec fails
            }
        default: // Parent process
            if(background_flag) // If the child process was a background command
            {
                printf("[Background PID: %d] %s is running\n", pid, args[0]); // Display to user the process id and command name that is running in background
            }
            else
            {
                waitpid(pid, &status, WUNTRACED); // Wait for the child process to finish if not a background command
            }
    }
}