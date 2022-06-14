#include <stdio.h>
#include <ncurses.h>

void init()
{
	initscr();
	noecho();
	keypad(stdscr, TRUE);
}

void draw_border(int maxx, int maxy)
{
	move(0, 0);
	hline('+', maxx);
	move(maxy-1, 0);
	hline('+', maxx);
	move(0, 0);
	vline('+', maxx);
	move(0, maxx-1);
	vline('+', maxx);
	move(0, 0);
}

int main()
{
	init();

	int maxx = getmaxx(stdscr);
	int maxy = getmaxy(stdscr);

	draw_border(maxx, maxy);

	getch();
	endwin();
	return 0;
}

