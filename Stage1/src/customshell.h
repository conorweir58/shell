#ifndef CUSTOMSHELL_H
#define CUSTOMSHELL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER 1024    // Max line buffer size
#define MAX_ARGS 64        // Max number of arguments
#define MAX_PATH 1024      // Max file path size
#define SEPARATORS " \t\n" // Token separators

extern char **environ;  // NULL terminated array of char *

#endif

// Function prototypes
char *get_prompt();
void execute_clear();
void execute_quit();
void execute_ls();
void execute_environ();
void command_not_found(char *args);

