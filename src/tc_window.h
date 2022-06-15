#ifndef __TC_WINDOW__H
#define __TC_WINDOW__H

#include <ncurses.h>
#include "pc_error.h"
#include "types.h"


void clr_win(WINDOW *w);
WINDOW *alloc_win(int h, int w, int y, int x, enum border_type t);

#endif
