#include "tc_window.h"
#include "pc_error.h"

static WINDOW *border_window(WINDOW *w, enum border_type type)
{
	switch (type) {
		case border_default:
			wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
			break;
		case border_type1:
			wborder(w, '|', '|', '-', '-', '+', '+', '+', '+');
			break;
		case border_type2:
			wborder(w, '|', '|', '-', '-', '+', '+', '*', '*');
			break;
		case border_type3:
			wborder(w, '|', '|', '-', '-', '*', '*', '*', '*');
			break;
		default:
			error_panic(stderr, "Unknown border type");
			break;
	}
	return w;
}

WINDOW *alloc_win(int h, int w, int y, int x, enum border_type type)
{
	WINDOW *win;
	win = newwin(h, w, y, x);
	if (win == NULL) {
		error_panic(stderr, "Couldn't alloc mem for WINDOW\n");
	}
	refresh();
	box(win, 0, 0);
	keypad(win, TRUE); /* enable keys like arrows, backspace, home etc. (not enabled by default for WINDOW) */
	wmove(win, 0, 1); /* init cursor at the start of the box */
	border_window(win, type);

	wrefresh(win);
	return win;
}


int write_window(WINDOW *w)
{
	return 0;
}

void clr_win(WINDOW *w)
{
	if (w) {
		wclear(w);
		wrefresh(w);
		delwin(w);
	}
}
