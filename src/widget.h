#ifndef __TC_WIDGET_H
#define __TC_WIDGET_H

#include <ncurses.h>

#include "tc_window.h"
#include "types.h"

typedef struct WindowProp {
	int height, width;
	int starty, startx;
} window_t;

typedef struct PromptWidget {
	window_t *s;
	WINDOW *w;
	char *question;
} prompt_t;

typedef struct InputWidget {
	window_t *s;
	WINDOW *w;
} input_t;

typedef struct MenuWidget {
	window_t *s;
	WINDOW *w;
	const char **options;
} menu_t;

WINDOW *GPromptWidget(prompt_t *t);
WINDOW *GInputWidget(input_t *t);
WINDOW *GMenuWidget(menu_t *t);
void FreeWidget(void *widget, enum free_type t);

window_t *GInitSz(int height, int width, int y, int x);

#endif
