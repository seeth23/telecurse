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


#define BASE_MENU_HEIGHT 2


void sigint_handler(int sig)
{
	endwin();
	exit(0);
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

void handler(int i) {
	//shutdown();
	//printf("%s WAS CHOSEN. %d option\n", actions[i], i+1);
}

/* must end up with NULL for easier looping */
//const char *fkeys_info[] = {"F1 - Help", "F5 - Exit", NULL};
const char *fkeys_info[] = {"F1 - Help", "F2 - Test", "F3 - Test2", "F4 - F4", "F5 - Exit", "F1 - Help", "F2 - Test", "F3 - Test2", "F4 - F4", "F5 - Exit", "F30 - WTF", "F90 - -90", NULL};
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

int main()
{
	init();
	print_fkeys();
#if DEBUG
	getch();
	shutdown();
	exit(0);
#endif
	signal(SIGINT, sigint_handler);
	/*int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);*/
	/* --------------Examples using widgets--------------- */
	/* ------------------PROMT-WIDGET--------------------- */
	prompt_t *name_prompt_widget = GPromptWidget("Enter name", 18, 5, 20, 3, 3, border_type2);
	name_prompt_widget->read(name_prompt_widget);
	/* After reading you can extract answer by writing widget->answer and save somewhere else it with strcpy */
	FreeWidget(name_prompt_widget, free_prompt);
	/* ------------------MENU-WIDGET---------------------- */
	/* Option list must end with NULL in case of internal implementation, either will segfault happen */
	/* And the size of these option list must be size-1, again in case of internal implementation: just to
	 * not print unnecessary line */
	const char *opt[] = { "Option1", "Option2", "Option3", "Option4", NULL };
	size_t opt_size = BLK_SIZE(opt);
	menu_t *menu_widget = GMenuWidget(opt, "Options are", opt_size, BASE_MENU_HEIGHT, 20, 1, 1, border_default);
	menu_widget->choose(menu_widget, NULL);
	FreeWidget(menu_widget, free_menu);
	/* ------------------INPUT-WIDGET--------------------- */
	/* input widget is not necessary
	 * because I already have prompt with possible NULL as first param
	 * so it literally looks like regular input window */
	/* ------------------INFO-WIDGET---------------------- */
	info_t *info_widget = GInfoWidget("Chat", 7, 15, 2, 2, border_default);
	FreeWidget(info_widget, free_info);
	/* --------------------------------------------------- */
	shutdown();
	//printf("Exited handler sucessfully!\n");
	return 0;
}
