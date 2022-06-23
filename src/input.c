#include "input.h"

winput_h *
input_init(WINDOW *win, size_t len, int starty, int startx)
{
	if (len <= 0)
		error_panic(stderr, "len cannot be <= 0 for winput_h!\n");
	winput_h *i = malloc(sizeof(winput_h));
	if (!i)
		error_panic(stderr, "Could not allocate memory for winput_h!\n");
	i->w = win;
	i->str_len = len;
	i->current_pos = 0;
	i->str = malloc(sizeof(char)*i->str_len);
	if (!i->str)
		error_panic(stderr, "Could not allocate memory for winput_h->str!\n");
	i->starty = starty;
	i->startx = startx;
	if (i->starty < 0 || i->starty < 0)
		error_panic(stderr, "winput->starty or winput_h->startx cannot be <= 0 for winput_h\n");
	i->cury = i->starty;
	i->curx = i->startx;
	return i;
}

void
free_winput(winput_h *t)
{
	if (!t)
		return;
	t->w = NULL;
	t->str = NULL;
	free(t);
}

static int
tc_isascii(int ch)
{
	return (ch >= 33 && ch <= 127) ? TRUE : FALSE;
}

static void
tc_putch(winput_h *t)
{
	t->curx++;
	t->str[t->current_pos++] = t->ch;
	waddch(t->w, t->str[t->current_pos-1]);
}

static void
tc_delch(winput_h *t)
{
	if (t->current_pos > 0) {
		wmove(t->w, t->cury, --t->curx);
		waddch(t->w, ' ');
		wmove(t->w, t->cury, t->curx);
		t->str[--t->current_pos] = '\0';
	}
}

static int
tc_isdelete(int ch)
{
	return (ch == KB_BACKSPACE || ch == KEY_BACKSPACE || ch == 127) ? TRUE : FALSE;
}

static int
tc_buf_overflow(winput_h *t)
{
	return t->current_pos >= t->str_len ? TRUE : FALSE;
}

static int
tc_isspace(int ch)
{
	return ch == ' ' ? TRUE : FALSE;
}

/* void (*filter)(int ch) callback checks if ch is f1-f12 key returns nothing
 * because it will handle if e.g. f5 for quit is pressed so no need to return */
char *
tc_wreadstr(winput_h *t, void (*filter)(int ch))
{
	int isascii;
	int isdelete;
	int isspace;
	if (!t) {
		error_panic(stderr, "winput_h cannot be (null)\n");
	}
	noecho();
	wmove(t->w, t->cury, t->curx);
	int count = 0;
	while ((t->ch = wgetch(t->w))) {
		if (t->ch == ERR && count == 0) {
			return NULL;
		}
		if (t->ch == '\n') {
			if (count == 0)
				continue;
			else break;
		}
		/* filter can be NULL */
		if (filter)
			filter(t->ch); /* check for *special* kb_keys */
		isdelete = tc_isdelete(t->ch);
		if (isdelete) {
			if (count > 0)
				count--;
			tc_delch(t);
			continue;
		}
		isspace = tc_isspace(t->ch);
		isascii = tc_isascii(t->ch);
		if (isascii || isspace) {
			if (tc_buf_overflow(t))
				continue;
			tc_putch(t);
			if (count < t->str_len)
				count++;
		} else /* if !isascii then continue */
			continue;
		refresh();
	}
	t->str[t->current_pos] = '\0';
	return t->str;
}
