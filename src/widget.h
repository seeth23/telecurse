#ifndef __TC_WIDGET_H
#define __TC_WIDGET_H

#include <ncurses.h>

#include "tc_window.h"
#include "types.h"
#include "input.h"

typedef struct WindowProp {
	int height, width;
	int starty, startx;
} window_t;

typedef struct PromptWidget {
	window_t *s;
	WINDOW *w;
	char *question;
	char *answer;
	size_t ans_size;
	void (*read)(struct PromptWidget *);
} prompt_t;

typedef struct InputWidget {
	window_t *s;
	WINDOW *w;
} input_t;

typedef struct MenuWidget {
	window_t *s;
	WINDOW *w;
	size_t current_item;
	size_t all_items;
	const char **options;
	void (*choose)(struct MenuWidget *, void (*handler)(int index));
} menu_t;

WINDOW *GPromptWidget(prompt_t *t, size_t sz);
WINDOW *GInputWidget(input_t *t);
WINDOW *GMenuWidget(menu_t *t, const char **opt);
WINDOW *GInfoWidget();

void FreeWidget(void *widget, enum free_type t);

window_t *GInitSz(int height, int width, int y, int x);

#endif
