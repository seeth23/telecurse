#ifndef __TC_INPUT__H
#define __TC_INPUT__H

#include <string.h>

#include "pc_error.h"
#include "tc_window.h"
#include "keymap.h"

typedef struct WInput {
	char *str;
	int ch;
	size_t current_pos;
	size_t str_len;
	WINDOW *w;
	int cury, curx;
	int starty, startx;
} winput_h;

char *tc_wreadstr(winput_h *t, void (*filter)(int ch));
winput_h *input_init(WINDOW *win, size_t len, int starty, int startx);
void free_winput(winput_h *t);

#endif

