#include "center.h"

void
center_cord(int height, int width, int *y, int *x)
{
	int term_maxy, term_maxx;
	getmaxyx(stdscr, term_maxy, term_maxx);
	*y = term_maxy/2-height/2;
	*x = term_maxx/2-width/2;
}
