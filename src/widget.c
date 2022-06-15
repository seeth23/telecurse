#include "widget.h"

static void ReadPrompt(prompt_t *t)
{
	tc_wgetnstr(t->w, t->answer, t->ans_size, 2, 1);
}

WINDOW *GPromptWidget(prompt_t *t, size_t size)
{
	window_t *inf = t->s;
	WINDOW *w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, border_type1);
	t->read = ReadPrompt;
	t->ans_size = size;
	t->answer = malloc(sizeof(char)*t->ans_size);
	if (!t->answer) {
		error_panic(stderr, "Could not alloc memory for buffer");
	}
	wprintw(w, "%s", t->question);
	wrefresh(w);
	inf = NULL;
	return w;
}

WINDOW *GInputWidget(input_t *t)
{
	return NULL;
}

WINDOW *GMenuWidget(menu_t *t)
{
	return NULL;
}

//WINDOW *G

window_t *GInitSz(int height, int width, int y, int x)
{
	window_t *t = malloc(sizeof(window_t));
	t->height = height;
	t->width = width;
	t->starty = y;
	t->startx = x;
	return t;
}

void FreeWidget(void *widget, enum free_type t)
{
	switch(t) {
		case free_input: {
			input_t *i = (input_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				clr_win(i->w);
			}
			break;
		}
		case free_menu: {
			menu_t *i = (menu_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				clr_win(i->w);
			}
			break;
		}
		case free_prompt: {
			prompt_t *i = (prompt_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				clr_win(i->w);
			}
			break;
		}
		default:
			break;
	}
	free(widget);
}

