#include "input.h"

//#include <regex.h>

static void delchar_w(WINDOW *w, int y, int x)
{
	int oldy=getcury(w), oldx=getcurx(w);
	wmove(w, y, x);
	waddch(w, ' ');
	wmove(w, oldy, oldx);
}

int tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x)
{
//#define ctrl(x)           ((x) & 0x1f)
#define BACKSPACE 263
	noecho();
	if (!buf) {
		error_panic(stderr, "Buf cannot be NULL\n");
	} else {
		memset(buf, 0, buf_len);
	}
	int cury, curx;
	int ch, count = 0;
	wmove(w, y, x);
	while (count>=0 && (ch = wgetch(w)) != '\n') {
		if (ch < 48) // disable 'unreadable' characters
			continue;
		getyx(w, cury, curx);
		if (curx > 1 && (ch == BACKSPACE || ch == KEY_BACKSPACE || ch == 127)) { /* delete */
			wmove(w, y, curx-1);
			delchar_w(w, y, curx-1);
			if (count>0)
				buf[--count]='\0';
			else
				error_panic(stderr, "Index out of array: 1\n");
		} else if (ch != BACKSPACE && ch != KEY_BACKSPACE && ch != 127) {
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
	return 0;
}
