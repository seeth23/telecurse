#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"
#include "input.h"
#include "widget.h"
#include "misc.h"
#include "math/center.h"

void rerender_window(WINDOW *w, enum border_type);

void print_fkeys();
void shutdown();
void init();
void sigint_handler(int sig);

/* must end up with NULL for easier looping */
const char *fkeys_info[] = {"F1 - Help", "F5 - Exit", NULL};

enum {
	NAME_PROMPT_HEIGHT = 4,
	NAME_PROMPT_WIDTH  = 20,
	CHAT_HEIGHT        = 8,
	CHAT_WIDTH         = 80,
	INPUT_CHAT_HEIGHT  = 4,
	INPUT_CHAT_WIDTH   = 100,
};

int main()
{
	init();
	print_fkeys();
#if DEBUG
	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);
	printw("y: %d x: %d", maxy, maxx);
	refresh();
	getch();
	shutdown();
	exit(0);
#endif
	signal(SIGINT, sigint_handler);
	/* TODO FUNCTION THAT CALCULATE CENTER OF THE SCREEN BASED ON HEIGHT WIDTH AND TERMINAL MAXY AND MAXX */
	/* --------------Examples using widgets--------------- */
	/* ------------------PROMT-WIDGET--------------------- */
	int prompt_starty;
	int prompt_startx;
	center_cord(NAME_PROMPT_HEIGHT, NAME_PROMPT_WIDTH, &prompt_starty, &prompt_startx);
	prompt_t *name_prompt_widget = GPromptWidget("Enter name", 18/*strlen*/, NAME_PROMPT_HEIGHT, NAME_PROMPT_WIDTH, prompt_starty, prompt_startx, border_type2);
	name_prompt_widget->read(name_prompt_widget);
	/* After reading you can extract answer by writing widget->answer and save somewhere else it with strcpy */
	FreeWidget(name_prompt_widget, free_prompt);
	/* ------------------MENU-WIDGET---------------------- */
	/* Option list must end with NULL in case of internal implementation, either will segfault happen */
	/* And the size of these option list must be size-1, again in case of internal implementation: just to
	 * not print unnecessary line */
#if 0
	const char *opt[] = { "Option1", "Option2", "Option3", "Option4", NULL };
	size_t opt_size = BLK_SIZE(opt);
	menu_t *menu_widget = GMenuWidget(opt, "Options are", opt_size, BASE_MENU_HEIGHT, 20, 1, 1, border_default);
	menu_widget->choose(menu_widget, NULL);
	FreeWidget(menu_widget, free_menu);
#endif
	/* ------------------INFO-WIDGET---------------------- */
	info_t *chat_widget = GInfoWidget("Chat", CHAT_HEIGHT, CHAT_WIDTH, getmaxy(stdscr)/2-CHAT_HEIGHT, getmaxx(stdscr)/2-CHAT_WIDTH/2, border_default);
	/* --------------------------------------------------- */
	prompt_t *input_prompt = GPromptWidget(NULL, 255, INPUT_CHAT_HEIGHT, INPUT_CHAT_WIDTH, getmaxy(stdscr)-20, getmaxx(stdscr)/2-INPUT_CHAT_WIDTH/2, border_type2);

	for (;;) {
		input_prompt->read(input_prompt);
		rerender_window(input_prompt->w, input_prompt->s->border_type);
	}

	FreeWidget(chat_widget, free_info);
	FreeWidget(input_prompt, free_prompt);
	shutdown();
	return 0;
}

void rerender_window(WINDOW *w, enum border_type bt)
{
	wclear(w);
	box(w, 0, 0);
	border_window(w, bt);
	wrefresh(w);
}

void sigint_handler(int sig)
{
}

void init()
{
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
}

void shutdown()
{
	endwin();
}

void print_fkeys()
{
	const char **list = fkeys_info;
	//int step;
	wmove(stdscr, 0, 0);
	while (*list) {
		//int size = OPTIONS_SIZE(fkeys_info)-1;
		//step = getmaxx(stdscr)/size;
		attron(A_REVERSE);
		addstr(*list);
		attroff(A_REVERSE);
		wmove(stdscr, 0, getcurx(stdscr) + 1);
		//wmove(stdscr, 0, getcurx(stdscr) + step - (strlen(*list)/2));
		list++;
	}
	wmove(stdscr, 0, 0);
}
