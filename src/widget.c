#include "widget.h"
#include "keymap.h"

#include "handlers/input_handlers.h"
#include "handlers/menu_handlers.h"
#include "misc.h"

// TODO make another .c file for 'methods'
// TODO make *FUNCTION* for printing horizontal list of f1-f12 actions on top of the program

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

/* TODO make separate file for 'callbacks' which I will include then */

static void read_prompt(prompt_t *t)
{
	winput_h *winput = input_init(t->w, t->ans_size, 1, 1);
	t->answer = tc_wreadstr(winput, handle_function_keys);
	free_winput(winput);
}

static void render_menu(menu_t *t)
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

static void choose_option(menu_t *t, void (*handler)(int index))
{
	int ch;
	int render = FALSE;
	while ((ch = wgetch(t->w))) {
		switch (ch) {
			case KEY_DOWN:
				if (t->current_item < t->all_items) {
					t->current_item++;
					render = TRUE;
				}
				break;
			case KEY_UP:
				if (t->current_item > 0) {
					t->current_item--;
					render = TRUE;
				}
				break;
			case KB_ENTER:
				if (handler)
					handler(t->current_item);
				return; /* return to exit function. If break used -- menu widget will not end */
			default:
				render = FALSE;
				break;
		}
		if (render)
			render_menu(t);
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
	window_t *inf = t->s; /* just for shortcut */

	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	t->ans_size = str_len;
	t->question = str;
	t->read = read_prompt;

	t->answer = malloc(sizeof(char)*t->ans_size);
	if (!t->answer) {
		error_panic(stderr, "Could not alloc memory for buffer\n");
	}
	/* t->question can be NULL, because it is not necessary, actually */
	if (t->question)
		wprintw(t->w, "%s", t->question);
	wrefresh(t->w);
	inf = NULL;

	t->p = new_panel(t->w);

	return t;
}

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
	t->s = GInitSz(height+opt_size-1, width, starty, startx);
	window_t *inf = t->s; /* just for shortcut */

	t->w = alloc_win(inf->height, inf->width, inf->starty, inf->startx, bt);
	mvwprintw(t->w, 0, 1, "%s", msg);

	t->options = opt;
	t->choose = choose_option;
	t->current_item = 0;
	t->all_items = opt_size-2; /* -1 for removing NULL line and -1 because starts from 0 so (-1)+(-1)=-2 */
	render_menu(t);
	inf = NULL;

	t->p = new_panel(t->w);
	return t;
}

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
	info_t *t = malloc(sizeof(info_t));
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
	t->p = new_panel(t->w);
	inf = NULL;
	return t;
}

static void tc_upd_pan()
{
	update_panels();
	doupdate();
}

void FreeWidget(void *widget, enum free_type t)
{
	switch(t) {
		case free_menu: {
			menu_t *i = (menu_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				del_panel(i->p);
				clr_win(i->w);
				tc_upd_pan();
			}
			return;
		}
		case free_prompt: {
			prompt_t *i = (prompt_t *)widget;
			if (i->s)
				free(i->s);
			if (i->w) {
				del_panel(i->p);
				clr_win(i->w);
				tc_upd_pan();
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
				del_panel(i->p);
				clr_win(i->w);
				tc_upd_pan();
			}
			break;
		}
	}
	free(widget);
}
