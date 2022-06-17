#include "input.h"

winput_h *input_init(WINDOW *win, size_t len, int starty, int startx)
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

void free_winput(winput_h *t)
{
	if (!t)
		return;
	t->w = NULL;
	t->str = NULL;
	free(t);
}
#if 0
static void delchar_w(WINDOW *w, int y, int x)
{
	int oldy=getcury(w), oldx=getcurx(w);
	wmove(w, y, x);
	waddch(w, ' ');
	wmove(w, oldy, oldx);
}


void tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x)
{
	noecho();
	if (!buf)
		error_panic(stderr, "Buf cannot be NULL\n");
	else
		memset(buf, 0, buf_len);
	int cury, curx;
	int ch, count = 0;
	wmove(w, y, x);
	while (count>=0 && (ch = wgetch(w)) != '\n') {
		if (ch < 48 || ch == KEY_DOWN || ch == KEY_UP || ch == KEY_LEFT || ch == KEY_RIGHT) // disable 'unreadable' characters
			continue;
		getyx(w, cury, curx);
		if (curx > 1 && (ch == KB_BACKSPACE || ch == KEY_BACKSPACE || ch == 127)) { /* delete */
			wmove(w, y, curx-1);
			delchar_w(w, y, curx-1);
			if (count > 0)
				buf[--count]='\0';
			else
				error_panic(stderr, "Index out of array: 1\n");
		} else if (ch != KB_BACKSPACE && ch != KEY_BACKSPACE && ch != 127) {
			if (count == buf_len)
				continue;
			if (count<buf_len)
				buf[count++]=ch;
			else
				error_panic(stderr, "Index out of array: 2\n");
			waddch(w, buf[count-1]);
		}
		wrefresh(w);
	}
	buf[count] = 0;
}
#endif

/* TODO Super Global Function for input parsing */
/* -------------------------------------- */

static int tc_isascii(int ch)
{
	return (ch >= 33 && ch <= 127) ? TRUE : FALSE;
}

static void tc_putch(winput_h *t)
{
	t->curx++;
	t->str[t->current_pos++] = t->ch;
	waddch(t->w, t->str[t->current_pos-1]);
}

static void tc_delch(winput_h *t)
{
	if (t->current_pos > 0) {
		wmove(t->w, t->cury, --t->curx);
		waddch(t->w, ' ');
		wmove(t->w, t->cury, t->curx);
		t->str[--t->current_pos] = '\0';
	}
}

static int tc_isdelete(int ch)
{
	return (ch == KB_BACKSPACE || ch == KEY_BACKSPACE || ch == 127) ? TRUE : FALSE;
}

static int tc_buf_overflow(winput_h *t)
{
	return t->current_pos >= t->str_len ? TRUE : FALSE;
}

static int tc_isspace(int ch)
{
	return ch == ' ' ? TRUE : FALSE;
}
//#define ctrl(x)           ((x) & 0x1f)
/* void (*filter)(int ch) callback checks if ch is f1-f12 key returns nothing
 * because it will handle if e.g. f5 for quit is pressed so no need to return */
char *tc_wreadstr(winput_h *t, void (*filter)(int ch))
{
	int isascii;
	int isdelete;
	int isspace;
	if (!t) {
		error_panic(stderr, "winput_h cannot be (null)\n");
	}
	noecho();
	wmove(t->w, t->cury, t->curx);
	while ((t->ch = wgetch(t->w)) != '\n') {
		/* filter can be NULL */
		if (filter)
			filter(t->ch); /* check for *special* kb_keys */
		isdelete = tc_isdelete(t->ch);
		if (isdelete) {
			tc_delch(t);
			continue;
		}
		isspace = tc_isspace(t->ch);
		isascii = tc_isascii(t->ch);
		if (isascii || isspace) {
			if (tc_buf_overflow(t))
				continue;
			tc_putch(t);
		} else /* if !isascii then continue */
			continue;
	}
	t->str[t->current_pos] = '\0';
	return t->str;
}
