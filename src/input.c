#include "input.h"

winput_h *input_init(WINDOW *win, size_t len, int y, int x)
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
	i->cury = y;
	i->curx = x;
	return i;
}

static void delchar_w(WINDOW *w, int y, int x)
{
	int oldy=getcury(w), oldx=getcurx(w);
	wmove(w, y, x);
	waddch(w, ' ');
	wmove(w, oldy, oldx);
}

/* TODO Super Global Function for input parsing */
#define KB_BACKSPACE 263

void tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x)
{
//#define ctrl(x)           ((x) & 0x1f)
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

/* -------------------------------------- */

/* TODO Check if ch is ascii or printable return 1 either 0 */
static int tc_isascii(int ch)
{
	return 0;
}

static void tc_putch(winput_h *t)
{
	waddch(t->w, t->ch);
	t->current_pos++;
}

static void tc_delch(winput_h *t)
{
	wmove(t->w, t->cury, t->curx-1);
	waddch(t->w, ' ');
	wmove(t->w, t->cury, t->curx);
	t->current_pos--;
}


/* TODO Check if ch is delete key return 1 either 0 */
static int tc_isdelete(int ch)
{
	return 0;
}

static int tc_buf_overflow(winput_h *t)
{
	if (t->current_pos >= t->str_len)
		return 0;
	return 1;
}

/* void (*filter)(int ch) callback checks if ch is f1-f12 key returns nothing
 * because it will handle if e.g. f5 for quit is pressed so no need to return */
void tc_wreadstr(winput_h *t, void (*filter)(int ch))
{
	int isascii;
	int isdelete;
	if (!t) {
		error_panic(stderr, "winput_h can not be (null)\n");
	}
	noecho();
	while ((t->ch = wgetch(t->w)) != '\n') {
		filter(t->ch);
		isascii = tc_isascii(t->ch);
		isdelete = tc_isdelete(t->ch);
		if (isdelete)
			tc_delch(t);
		if (isascii) {
			if (tc_buf_overflow(t))
				continue;
			tc_putch(t);
		}
		else
			continue;
	}
}
