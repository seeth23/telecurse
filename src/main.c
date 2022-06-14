#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"

/* TODO Separate all these functions and structs to related files */
/* TODO Make struct for windows */
/* TODO Upper panel for shortcuts like 'exit' */

int current_users;

typedef struct term_info termi_t;

void filter_input()
{
}

void init()
{
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	border('|', '|', '-', '-', '+', '+', '+', '+');
}

void shutdown()
{
	endwin();
}

const char *meeting_prompt(int maxy, int maxx)
{
#define NAME_SIZE 50
	char *name = malloc(NAME_SIZE);
	WINDOW *meeting = alloc_win(10, 30, maxy/2-(10/2), maxx/2-15, border_type1);
	wprintw(meeting, "What is your name?\n");
	wrefresh(meeting);
	wmove(meeting, 2, 1);
	while (wgetnstr(meeting, name, NAME_SIZE)==0&&strcmp(name, "")==0) {
		if (strcmp(name, "")==0) {
			mvwprintw(meeting, 1, 1, "Name cannot be empty!");
			wmove(meeting, 2, 1);
		}
	}
	clr_win(meeting);
	return name;
}
#define MAX_USERS 32
int fill_user_table(const char *users[MAX_USERS], const char *new_name)
{
	int i;
	for (i=0; i<MAX_USERS; i++) {
		if (!users[i]) {
			users[i]=new_name;
			break;
		}
	}
	current_users++;
	return i;
}

void fill_user_win(WINDOW *w, const char *users[MAX_USERS])
{
	for (int i=0; i<current_users; ++i) {
		if (users[i])
			wprintw(w, "%s", users[i]);
	}
}

void main_loop()
{
}

int main()
{
	init();
	int maxx = getmaxx(stdscr);
	int maxy = getmaxy(stdscr);

	const char *users_table[32];
	memset(&users_table, 0, sizeof(users_table));

#define INPUT_WINY 3
#define INPUT_WINX maxx-40
#define CHAT_WINY 10
#define CHAT_WINX maxx-40
#define USERS_WINY ((INPUT_WINY)+(CHAT_WINY))
#define USERS_WINX 30

	refresh();
	const char *name = meeting_prompt(maxy, maxx);
	fill_user_table(users_table, name);

#if 0
	endwin();
	printf("index: %d\n", index);
	printf("all users: %d\n", current_users);
	printf("[index]: %s\n", users_table[index]);
	for (int j=0; j<current_users; j++) {
		free((char*)users_table[j]);
		users_table[j]=NULL;
	}
	exit(0);
#endif

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
	int ret;
	while ((ret = wgetnstr(input, str, MSG_SIZE) == 0) && (strcmp(str, "STOP")!=0)) {
		wprintw(chat, "\t%s: %s\n", name, str);
		wborder(chat, '|', '|', '-', '-', '+', '+', '+', '+');
		wrefresh(chat);
		delwin(input);
		input = alloc_win(INPUT_WIN);
	}

	delwin(chat);
	delwin(input);
	//free((char*)name);
	for (int j=0; j<current_users; j++) {
		free((char*)users_table[j]);
		users_table[j]=NULL;
	}
	shutdown();
	return 0;
}
