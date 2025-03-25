/*
STUDENT NAME: CONOR WEIR
STUDENT ID: 23418374
ACADEMIC INTEGRITY STATEMENT: I acknowledge DCU's academic integrity policy.
*/

#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#include "customshell.h"

#endif

// Function prototypes

// Command functions
void execute_clear(); // Function to execute the "clear" command -> clears the terminal screen
void execute_quit(); // Function to execute the "quit" command -> exits the shell
void execute_ls(char *path); // Function to execute the "dir" command -> lists all files and directories in the current directory or a given path
void execute_environ(); // Function to execute the "environ" command -> lists all environment variables/strings
void execute_cd(char *path); // Function to execute the "cd" command -> changes the current working directory or returns the current working directory
void execute_echo(char **args); // Function to execute the "echo" command -> prints the arguments passed to it
void execute_pause(); // Function to execute the "pause" command -> prompts the user to press enter to continue
void execute_help(); // Function to execute the "help" command -> displays the user manual
void execute_external_command(char **args); // Function to execute external commands
