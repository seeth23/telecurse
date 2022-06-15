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
	const char *question;
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
	void (*choose)(struct MenuWidget *, void (*handler)(const char **actions, int index));
} menu_t;

//WINDOW *GPromptWidget(prompt_t *t, size_t sz);
prompt_t *GPromptWidget(const char *str, size_t str_len, int height, int width, int starty, int startx, enum border_type t);

//WINDOW *GMenuWidget(menu_t *t, const char **opt, const char *msg);
menu_t *GMenuWidget(const char **opt, const char *msg, size_t opt_size, int height, int width, int starty, int startx, enum border_type t);

WINDOW *GInputWidget(input_t *t);
WINDOW *GInfoWidget();

void FreeWidget(void *widget, enum free_type t);

//window_t *GInitSz(int height, int width, int y, int x);
//static window_t *GInitSz(int height, int width, int y, int x);

#endif
