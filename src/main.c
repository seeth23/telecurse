#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"
#include "input.h"
#include "widget.h"

#define BASE_MENU_HEIGHT 2

#define OPTIONS_SIZE(opts) (sizeof(opts)/sizeof(opt[0]))

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

void handler(const char **actions, int i) {
	//shutdown();
	printf("%s WAS CHOSEN. %d option\n", actions[i], i+1);
}

int main()
{
	init();
	signal(SIGINT, sigint_handler);
	/*int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);*/
	char name[30];
	/* --------------Examples using widgets--------------- */
	/* ------------------PROMT-WIDGET--------------------- */
	prompt_t *name_prompt_widget = GPromptWidget("Enter name", 18, 5, 20, 1, 1, border_type2);
	name_prompt_widget->read(name_prompt_widget);
	/* After reading you can extract answer by writing widget->answer and save somewhere else it with strcpy */
	strcpy(name, name_prompt_widget->answer);
	FreeWidget(name_prompt_widget, free_prompt);
	/* ------------------MENU-WIDGET---------------------- */
	/* Option list must end with NULL in case of internal implementation, either will segfault happen */
	/* And the size of these option list must be size-1, again in case of internal implementation: just to
	 * not print unnecessary line */
	const char *opt[] = { "Option1", "Option2", "Option3", "Option4", NULL };
	size_t size = OPTIONS_SIZE(opt);
	menu_t *menu_widget = GMenuWidget(opt, "Options are", size, BASE_MENU_HEIGHT, 20, 1, 1, border_default);
	menu_widget->choose(menu_widget, handler);
	FreeWidget(menu_widget, free_menu);
	/* ------------------INPUT-WIDGET--------------------- */
	/* input widget is not necessary
	 * because I already have prompt with possible NULL as first param */
	/* ------------------INFO-WIDGET---------------------- */
	info_t *info_widget = GInfoWidget("Chat", 7, 15, 2, 2, border_default);
	FreeWidget(info_widget, free_info);
	/* --------------------------------------------------- */

	shutdown();
	printf("Exited handler sucessfully!\n");
	printf("name: %s\n:", name);
	printf("size: %ld\n", size);
	return 0;
	}
