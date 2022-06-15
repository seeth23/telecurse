#ifndef __TC_INPUT__H
#define __TC_INPUT__H

#include "pc_error.h"
#include <string.h>


int tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x);

#endif

