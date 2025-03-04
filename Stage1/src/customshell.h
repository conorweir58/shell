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
void start_up_shell();
char *get_prompt();
void execute_batch_file(char *file);
void execute_command(char **args);
// void tokenize(char *input, char **args);

