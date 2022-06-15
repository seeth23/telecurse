#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//#include <iniparser.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"

#include <unistd.h>

void sigint_handler(int sig)
{
	// TODO POP UP MENU WIDGET WITH OK AND CANCEL OPTIONS
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

/* Bones: */

typedef struct PromptWidget {
	int height, weight;
	int starty, startx;
	WINDOW *w;
	char *question;
} prompt_t;

typedef struct InputWidget {
	int height, weight;
	int starty, startx;
	WINDOW *w;
} input_t;

typedef struct MenuWidget {
	int height, weight;
	int starty, startx;
	WINDOW *w;
	const char **options;
} menu_t;

WINDOW *PromptWidget(int h, int w, int y, int x, const char *question)
{
	WINDOW *win = newwin(h, w, y, x);
	return NULL;
}


WINDOW *InputWidget()
{
	return NULL;
}

WINDOW *MenuWidget()
{
	return NULL;
}

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
	//border('|', '|', '-', '-', '+', '+', '+', '+');
}

void shutdown()
{
	endwin();
}

#define NAME_SIZE 50
const char *meeting_prompt(int maxy, int maxx)
{
	char *name = malloc(NAME_SIZE);
	WINDOW *meeting = alloc_win(10, 30, maxy/2-(10/2), maxx/2-15, border_type1);
	wrefresh(meeting);
	wprintw(meeting, "What is your name?\n");
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

int kbhit(WINDOW *w, int *ch)
{
	int r;
	nodelay(w, TRUE);
	noecho();
	*ch = wgetch(w);
	if(*ch == ERR)
		r = FALSE;
	else {
		r = TRUE;
	 	ungetch(*ch);
	}
	echo();
	nodelay(w, FALSE);
	return(r);
}

int mywgetnstr(WINDOW *w, char *buf, size_t buf_len);

int main()
{
	init();
	signal(SIGINT, sigint_handler);
	const char *users_table[32];
	memset(&users_table, 0, sizeof(users_table));

	int maxx = getmaxx(stdscr);
	int maxy = getmaxy(stdscr);


/* TODO calculate placement coordinates in separate variables so it looks more pretty */
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
	/* CLIENT MUST NOT UPDATE CHAT: SERVER WILL SEND EVERY CLIENT A MESSAGE TO RENDER. THIS IS JUST EXAMPLE FOR GUI 
	 * TODO separate server program to handle client input and data */

	//while ((ret = wgetnstr(input, str, MSG_SIZE) == 0) && (strcmp(str, "STOP")!=0)) {
	//	wprintw(chat, "\t%s: %s\n", name, str);
	//	wborder(chat, '|', '|', '-', '-', '+', '+', '+', '+');
	//	wrefresh(chat);
	//	delwin(input);
	//	input = alloc_win(INPUT_WIN);
	//}


	int ch;
	int running=1;
	while (running) {
		/*if (kbhit(input, &ch)) {
		switch (ch) {
				case KEY_PPAGE:
					running=0;
					goto endloop;
			}*/
			
			//ret=wgetnstr(input, str, MSG_SIZE);
		ret=mywgetnstr(input, str, MSG_SIZE);
		if (strlen(str)>0) {
			wprintw(chat, "\t%s: %s\n", name, str);
			wborder(chat, '|', '|', '-', '-', '+', '+', '+', '+');
			wrefresh(chat);
			//delwin(input);
			clr_win(input);
			input = alloc_win(INPUT_WIN);
		}
		//}
	}

	delwin(chat);
	delwin(input);

	for (int j=0; j<current_users; j++) {
		free((char*)users_table[j]);
		users_table[j]=NULL;
	}

	shutdown();
	return 0;
}

void delchar_w(WINDOW *w, int y, int x)
{
	int oldy=getcury(w), oldx=getcurx(w);
	wmove(w, y, x);
	waddch(w, ' ');
	wmove(w, oldy, oldx);
}

void clearln(WINDOW *w, int endx)
{
	for (int i=0; i<endx; i++) {
		waddch(w, ' ');
	}
}

int mywgetnstr(WINDOW *w, char *buf, size_t buf_len)
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

	while (count<buf_len && (ch = wgetch(w)) != '\n') {
		if (ch == '\n') {
			clearln(w, count);
		}
		if (getcurx(w)>getbegx(w) && (ch == BACKSPACE || ch == KEY_BACKSPACE || ch == 127)) {
			wmove(w, 1, getcurx(w)-1);
			delchar_w(w, 1, getcurx(w));
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
