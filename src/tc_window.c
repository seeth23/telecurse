#include "tc_window.h"

WINDOW *alloc_win(int h, int w, int y, int x, enum border_type type)
{
	WINDOW *win;
	win = newwin(h, w, y, x);
	if (win==NULL) {
		error_panic(stderr, "Couldn't alloc mem for WINDOW\n");
	}
	refresh();
	box(win, 0, 0);
	keypad(win, TRUE); /* enable keys like arrows, f1-12, backspace etc. (not enabled by default) */
	wmove(win, 1, 1); /* init cursor at the start of the box */
	switch (type) {
		case border_default:
			wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
			break;
		case border_type1:
			wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
			break;
		case border_type2:
			wborder(win, '|', '|', '-', '-', '+', '+', '*', '*');
			break;
		case border_type3:
			wborder(win, '|', '|', '-', '-', '*', '*', '*', '*');
			break;
		default:
			break;
	}
	wrefresh(win);
	return win;
}

void clr_win(WINDOW *w)
{
	wclear(w);
	wrefresh(w);
	delwin(w);
}
