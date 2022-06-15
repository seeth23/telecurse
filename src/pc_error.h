#ifndef __TC_ERROR__H
#define __TC_ERROR__H

#include <ncurses.h>
#include <stdlib.h>

void error_panic(FILE *f, const char *fmt, ...);

#endif
