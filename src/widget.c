#include "widget.h"

// TODO make another .c file for 'methods'

static void ReadPrompt(prompt_t *t)
{
	tc_wgetnstr(t->w, t->answer, t->ans_size, 2, 1);
}

static void RenderMenu(menu_t *t, int index)
{
	const char **list = t->options;
	size_t line = 0;
	while (*list) {
		if (line == index)
			wattron(t->w, A_REVERSE);
		else
			wattroff(t->w, A_REVERSE);
		mvwprintw(t->w, line++ + 1, 6, "%s", *list++);
		wrefresh(t->w);
	}
}
#define KB_ENTER 10
static void ChooseOption(menu_t *t, void (*handler)(int index))
{
	const char **list = t->options;
	size_t index = 0;
	int ch;
	while ((ch = wgetch(t->w)) && index >= 0) {
		switch (ch) {
			case KEY_DOWN:
				if (index < t->all_items-1)
					index++;
				break;
			case KEY_UP:
				if (index > 0)
					index--;
				break;
			case KB_ENTER:
				handler(index);
				break;
		}
		RenderMenu(t, index);
	}
}

WINDOW *GPromptWidget(prompt_t *t, size_t size)
{
	window_t *inf = t->s;
	WINDOW *w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, border_type2);
	t->ans_size = size;
	t->answer = malloc(sizeof(char)*t->ans_size);
	if (!t->answer) {
		error_panic(stderr, "Could not alloc memory for buffer");
	}
	wprintw(w, "%s", t->question);
	wrefresh(w);
	inf = NULL;
	t->read = ReadPrompt;
	return w;
}

WINDOW *GInputWidget(input_t *t)
{
	return NULL;
}

WINDOW *GMenuWidget(menu_t *t, const char **opt)
{
	window_t *inf = t->s;
	WINDOW *w = alloc_win(inf->height, inf->width, inf->starty, inf->starty, border_type1);
	t->options = opt;
	t->choose = ChooseOption;
	t->current_item = 0;
	t->all_items = 0;
	size_t line = 0;
	while (*opt) {
		if (line == 0)
			wattron(w, A_REVERSE);
		else
			wattroff(w, A_REVERSE);
		mvwprintw(w, line++ + 1, 6, "%s", *opt++);
		t->all_items++;
	}
	wrefresh(w);
	return w;
}

WINDOW *GInfoWidget()
{

	return NULL;
}

window_t *GInitSz(int height, int width, int y, int x)
{
	window_t *t = malloc(sizeof(window_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory!");
	}
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

