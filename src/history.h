#ifndef __HISTORY_H__
#define __HISTORY_H__

#define MAX_HISTORY 512

#include <stdlib.h>
#include <string.h>

#include "pc_error.h"

int save_history(const char *msg);
char **get_history();
int remove_history();
void free_history();
int history_size();
void zero_history();

#endif
