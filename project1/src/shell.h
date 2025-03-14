#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "util.h"

#define MAX_HISTORY 100
#define MAX_INPUT_SIZE 1024 * 1024

/* function prototype */
void enable_raw_mode();
void disable_raw_mode();
void clear_line(int position);
void refresh_line(const char *prompt, char *buffer);
void add_to_history(const char *cmd);
char* read_line(const char *prompt);
void cleanup_history();

#endif // SHELL_H_