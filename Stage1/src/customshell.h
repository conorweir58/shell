/*
STUDENT NAME: CONOR WEIR
STUDENT ID: 23418374
ACADEMIC INTEGRITY STATEMENT: I acknowledge DCU's academic integrity policy.
*/

#ifndef CUSTOMSHELL_H
#define CUSTOMSHELL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER 1024    // Max line buffer size
#define MAX_ARGS 64        // Max number of arguments
#define MAX_PATH 1024      // Max file path size
#define SEPARATORS " \t\n\r" // Token separators -> space, tab, newline, return

extern char **environ;  // NULL terminated array of char *

#endif

//
// Function prototypes
//

// Helper functions
void start_up_shell(); // Function to display the welcome message box
void set_shell_path(); // Function to set the SHELL environment variable to the path of the shell executable
char *get_prompt(); // Function to get the shell prompt based on the current working directory
void execute_batch_file(char *file); // Function to execute a batch file
void execute_command(char **args); // Function to check and execute a given command

