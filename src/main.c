#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"
#include "input.h"
#include "widget.h"


void sigint_handler(int sig)
{
	endwin();
	exit(0);
}

/* TODO Separate all these functions and structs to related files */
/* TODO Make struct for windows */
/* TODO Upper panel for shortcuts like 'exit' */
/* TODO struct for 'chat page' and linked list it */
/* TODO make struct for current session with sizes, and draw
 * windows with its help */
/* TODO make template functions like: "prompt function" with question parameter, common window with data to display, input window, menu window with option list */
/* CLIENT MUST NOT UPDATE CHAT: SERVER WILL SEND EVERY CLIENT A MESSAGE TO RENDER. THIS IS JUST EXAMPLE FOR GUI
 * TODO separate server program to handle client input and data */

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

#if 0
const char *meeting_prompt(int maxy, int maxx)
{
	char *name = malloc(NAME_SIZE);
	WINDOW *meeting = alloc_win(10, 30, maxy/2-(10/2), maxx/2-15, border_type1);
	wrefresh(meeting);
	wprintw(meeting, "Enter name:\n");
	wmove(meeting, 2, 1);
	while (wgetnstr(meeting, name, NAME_SIZE)==0 && strcmp(name, "")==0) {
		if (strcmp(name, "")==0) {
			mvwprintw(meeting, 1, 1, "Name cannot be empty!");
			wmove(meeting, 2, 1);
		}
	}
	clr_win(meeting);
	return name;
}
#endif

void handler(const char **actions, int i) {
	endwin();
	printf("%s WAS CHOSEN\n", actions[i]);
	//exit(0);
}

int main()
{
	init();
	signal(SIGINT, sigint_handler);
	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);

	/* ------------------PROMT-WIDGET--------------------- */
	prompt_t *name_prompt_widget = GPromptWidget("Enter name", 18, 5, 20, maxy/2-5, maxx/2-20);
	name_prompt_widget->read(name_prompt_widget);
	FreeWidget(name_prompt_widget, free_prompt);
	/* ------------------MENU-WIDGET--------------------- */
#if 0
	menu_t *menu_widget = malloc(sizeof(menu_t));
	if (!menu_widget) {
		error_panic(stderr, "Could not allocate memory for menu widget\n");
	}
	menu_widget->s = GInitSz(2+size, 20, 2, 80);
#endif
	const char *opt[] = { "Option1", "Option2", "Option3", "Option4" };
	size_t size = sizeof(opt) / sizeof(opt[0]);
	menu_t *menu_widget = GMenuWidget(opt, "You have", size, 2, 20, 2, 80);
	menu_widget->choose(menu_widget, handler);
	FreeWidget(menu_widget, free_menu);

	shutdown();
	return 0;
#if 0
#define INPUT_WINY 3
#define INPUT_WINX maxx-40
#define CHAT_WINY 10
#define CHAT_WINX maxx-40
#define USERS_WINY ((INPUT_WINY)+(CHAT_WINY))
#define USERS_WINX 30

	refresh();
	const char *name = meeting_prompt(maxy, maxx);
	fill_user_table(users_table, name);

#define INPUT_WIN INPUT_WINY, INPUT_WINX, getbegy(chat)+getmaxy(chat), 1, border_default
#define CHAT_WIN CHAT_WINY, CHAT_WINX, 2, 1, border_type1
#define USERS_WIN USERS_WINY, USERS_WINX, 2, maxx-35, border_default

	WINDOW *users = alloc_win(USERS_WIN);
	WINDOW *chat  = alloc_win(CHAT_WIN);
	WINDOW *input = alloc_win(INPUT_WIN);
	fill_user_win(users, users_table);

	scrollok(chat, TRUE);
	wrefresh(chat);
	wrefresh(input);
	wrefresh(users);

#define MSG_SIZE 192

	char str[MSG_SIZE];
	int running=1;
	while (running) {
		tc_wgetnstr(input, str, MSG_SIZE, 1, 1);
		if (strlen(str)>0) {
			wprintw(chat, "\t%s: %s\n", name, str);
			wborder(chat, '|', '|', '-', '-', '+', '+', '+', '+');
			wrefresh(chat);
			clr_win(input);
			input = alloc_win(INPUT_WIN);
		}
	}
	delwin(chat);
	delwin(input);
	for (int j=0; j<current_users; j++) {
		free((char*)users_table[j]);
		users_table[j]=NULL;
	}
#endif
}

