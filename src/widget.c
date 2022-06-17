#include "widget.h"
#include "keymap.h"

// TODO make another .c file for 'methods'

static window_t *GInitSz(int height, int width, int y, int x)
{
	window_t *t = malloc(sizeof(window_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory for window_t!\n");
	}
	t->height = height;
	t->width = width;
	t->starty = y;
	t->startx = x;
	return t;
}

void foo(int ch) {}

static void ReadPrompt(prompt_t *t)
{
	//tc_wgetnstr(t->w, t->answer, t->ans_size, 1, 1);
	winput_h *winput = input_init(t->w, t->ans_size, 1, 1);
	t->answer = tc_wreadstr(winput, foo);
	free_winput(winput);
}

static void RenderMenu(menu_t *t)
{
	const char **list = t->options;
	size_t line = 0;
	while (*list) {
		if (line == t->current_item)
			wattron(t->w, A_REVERSE);
		else
			wattroff(t->w, A_REVERSE);
		/* adding 1 just in case to start from 1st line instead of 0 */
		mvwprintw(t->w, 1+line++, 6, "%s", *list);
		list++;
	}
	wrefresh(t->w);
}

static void ChooseOption(menu_t *t, void (*handler)(const char **actions, int index))
{
	int ch;
	while ((ch = wgetch(t->w)) /*&& index >= 0*/) {
		switch (ch) {
			case KEY_DOWN:
				if (t->current_item < t->all_items)
					t->current_item++;
				break;
			case KEY_UP:
				if (t->current_item > 0)
					t->current_item--;
				break;
			case KB_ENTER:
				handler(t->options, t->current_item);
				return; /* return to exit function. If break used -- menu widget will not end */
			case KEY_PPAGE:
				return;
			default:
				break;
		}
		RenderMenu(t);
	}
}

prompt_t *GPromptWidget(
		const char *str,
		size_t str_len,
		int height,
		int width,
		int starty,
		int startx,
		enum border_type bt)
{
	prompt_t *t = malloc(sizeof(prompt_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory for prompt widget\n");
	}
	t->s = GInitSz(height, width, starty, startx);
	window_t *inf = t->s;

	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	t->ans_size = str_len;
	t->question = str;
	t->read = ReadPrompt;

	t->answer = malloc(sizeof(char)*t->ans_size);
	if (!t->answer) {
		error_panic(stderr, "Could not alloc memory for buffer\n");
	}
	/* t->question can be NULL, because it is not necessary, actually */
	if (t->question)
		wprintw(t->w, "%s", t->question);
	wrefresh(t->w);
	inf = NULL;
	return t;
}

#if 0
WINDOW *GPromptWidget(prompt_t *t, const char *str, size_t size)
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
#endif


#if 0
input_t *GInputWidget(
		int height,
		int width,
		int starty,
		int startx,
		enum border_type bt)
{
	input_t *t = malloc(sizeof(input_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory for input widget\n");
	}
	t->s = GInitSz(height, width, starty, startx);
	window_t *inf = t->s; /* just for shortcut */
	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	return t;
}
#endif

menu_t *GMenuWidget(
		const char **opt,
		const char *msg,
		size_t opt_size,
		int height,
		int width,
		int starty,
		int startx,
		enum border_type bt)
{
	menu_t *t = malloc(sizeof(menu_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory for menu widget\n");
	}
	t->s = GInitSz(height+opt_size, width, starty, startx);
	window_t *inf = t->s; /* just for shortcut */

	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	mvwprintw(t->w, 0, 1, "%s", msg);

	t->options = opt;
	t->choose = ChooseOption;
	t->current_item = 0;
	t->all_items = opt_size-1; /* -1 for removing NULL line */

	RenderMenu(t);
#if 0
	size_t line = 0;
	while (*opt) {
		if (line == 0)
			wattron(t->w, A_REVERSE);
		else
			wattroff(t->w, A_REVERSE);
		mvwprintw(t->w, 1+line++, 6, "%s", *opt++);
		wrefresh(t->w);
	}
#endif
	return t;
}

#if 0
WINDOW *GMenuWidget(menu_t *t, const char **opt, const char *msg)
{
	window_t *inf = t->s;
	WINDOW *w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, border_type1);
	mvwprintw(w, 0, 1, "%s", msg);
	t->options = opt;
	t->choose = ChooseOption;
	t->current_item = 0;
	t->all_items = 0;
	size_t line = 0;
	const char **list = opt;
	while (*list) {
		if (line == 0)
			wattron(w, A_REVERSE);
		else
			wattroff(w, A_REVERSE);
		t->all_items++;
		mvwprintw(w, line++ + 1, 6, "%s", *list);
		list++;
		wrefresh(w);
	}
	return w;
}
#endif

static void WriteInfo(info_t *t, const char *str)
{
	wmove(t->w, 1, 1);
}

info_t *GInfoWidget(
		const char *window_name,
		int height,
		int width,
		int starty,
		int startx,
		enum border_type bt)
{
	info_t *t = malloc(sizeof(input_t));
	if (!t) {
		error_panic(stderr, "Could not allocate memory for input widget\n");
	}
	t->s = GInitSz(height, width, starty, startx);
	window_t *inf = t->s; /* just for shortcut */
	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	t->msg_num = 0;
	t->write = WriteInfo;
	if (window_name) {
		wprintw(t->w, "%s", window_name);
		wrefresh(t->w);
	}
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
			return;
		}
		case free_prompt: {
			prompt_t *i = (prompt_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				clr_win(i->w);
			}
			if (i->answer)
				free(i->answer);
			break;
		}
		case free_info: {
			info_t *i = (info_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				clr_win(i->w);
			}
			break;
		}
	}
	free(widget);
}
