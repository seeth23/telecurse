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

int tc_wgetnstr(WINDOW *w, char *buf, size_t buf_len, int y, int x)
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
	wmove(w, y, x);
	while (count<buf_len && (ch = wgetch(w)) != '\n') {
		if (getcurx(w)>1 && (ch == BACKSPACE || ch == KEY_BACKSPACE || ch == 127)) {
			wmove(w, y, getcurx(w)-1);
			delchar_w(w, y, getcurx(w));
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
