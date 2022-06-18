#ifndef __TC_WINDOW_H__
#define __TC_WINDOW_H__

#include <ncurses.h>
#include "pc_error.h"
#include "types.h"


void clr_win(WINDOW *w);
WINDOW *alloc_win(int h, int w, int y, int x, enum border_type t);
WINDOW *border_window(WINDOW *w, enum border_type type);

#endif
