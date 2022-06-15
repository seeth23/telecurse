#include "input.h"


static void delchar_w(WINDOW *w, int y, int x)
{
	int oldy=getcury(w), oldx=getcurx(w);
	wmove(w, y, x);
	waddch(w, ' ');
	wmove(w, oldy, oldx);
}

static void clearln(WINDOW *w, int endx)
{
	for (int i=0; i<endx; i++) {
		waddch(w, ' ');
	}
}

int mywgetnstr(WINDOW *w, char *buf, size_t buf_len)
{
#define ctrl(x)           ((x) & 0x1f)
#define BACKSPACE 263
	noecho();
	if (!buf) {
		error_panic(stderr, "Buf cannot be NULL\n");
	} else {
		memset(buf, 0, buf_len);
	}
	int ch, count = 0;

	while (count<buf_len && (ch = wgetch(w)) != '\n') {
		if (ch == '\n') {
			clearln(w, count);
		}
		if (getcurx(w)>getbegx(w) && (ch == BACKSPACE || ch == KEY_BACKSPACE || ch == 127)) {
			wmove(w, 1, getcurx(w)-1);
			delchar_w(w, 1, getcurx(w));
			buf[--count]='\0';
			wrefresh(w);
		} else if (ch != BACKSPACE && ch != KEY_BACKSPACE && ch != 127) {
			buf[count++]=(char)ch;
			waddch(w, ch);
			wrefresh(w);
		}
	}
	return 0;
}
