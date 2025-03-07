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
void execute_clear();
void execute_quit();
void execute_ls(char *path);
void execute_environ();
void execute_cd(char *path);
void execute_echo(char **args);
void execute_pause();
void execute_help();
void command_not_found(char *args);