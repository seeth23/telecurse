#ifndef __TC_INPUT__H
#define __TC_INPUT__H

#include "pc_error.h"
#include <string.h>

typedef struct WInput {
	char *str;
	int ch;
	size_t current_pos;
	size_t str_len;
	WINDOW *w;
	int cury, curx;
	int starty, startx;
} winput_h;

void tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x);

char *tc_wreadstr(winput_h *t, void (*filter)(int ch));

winput_h *input_init(WINDOW *win, size_t len, int starty, int startx);

void free_winput(winput_h *t);
#endif

