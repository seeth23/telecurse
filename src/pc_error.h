#ifndef __TC_ERROR__
#define __TC_ERROR__

#include <ncurses.h>
#include <stdlib.h>

void error_panic(FILE *f, const char *fmt, ...);

#endif
