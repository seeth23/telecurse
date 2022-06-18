#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"
#include "input.h"
#include "widget.h"
#include "misc.h"
#include "math/center.h"

static void rerender_window(WINDOW *w, enum border_type);

static void print_fkeys();
static void tc_shutdown();
static void init();

/* must end up with NULL for easier looping */
static const char *fkeys_info[] = {"F1 - Help", "F2 - Find", "F3 - Users", "F5 - Exit", NULL};

enum {
	NAME_PROMPT_HEIGHT = 4,
	NAME_PROMPT_WIDTH  = 20,
	CHAT_HEIGHT        = 20,
	CHAT_WIDTH         = 80,
	INPUT_CHAT_HEIGHT  = 4,
	INPUT_CHAT_WIDTH   = 100,
};

typedef struct CenterCoordinates {
	int y, x;
} centercords_t;

int main()
{
	init();
	print_fkeys();

	char name[18];
	centercords_t nameprompt_cords;
	centercords_t chat_cords;
	centercords_t input_cords;

	center_cord(NAME_PROMPT_HEIGHT, NAME_PROMPT_WIDTH, &nameprompt_cords.y, &nameprompt_cords.x);
	center_cord(CHAT_HEIGHT, CHAT_WIDTH, &chat_cords.y, &chat_cords.x);
	center_cord(INPUT_CHAT_HEIGHT, INPUT_CHAT_WIDTH, &input_cords.y, &input_cords.x);

	prompt_t *name_prompt_widget = GPromptWidget("Enter name", 18, NAME_PROMPT_HEIGHT, NAME_PROMPT_WIDTH, nameprompt_cords.y, nameprompt_cords.x, border_type2);
	name_prompt_widget->read(name_prompt_widget);
	strcpy(name, name_prompt_widget->input);
	FreeWidget(name_prompt_widget, free_prompt);

	info_t *chat_widget = GInfoWidget("Chat", CHAT_HEIGHT, CHAT_WIDTH, chat_cords.y, chat_cords.x, border_default);
	prompt_t *input_prompt = GPromptWidget(NULL, 255, INPUT_CHAT_HEIGHT, INPUT_CHAT_WIDTH, input_cords.y+CHAT_HEIGHT, input_cords.x, border_type2);

	for (;;) {
		input_prompt->read(input_prompt);
		chat_widget->write(chat_widget, input_prompt->input, name);
		if (input_prompt->input)
			free(input_prompt->input);
		rerender_window(input_prompt->w, input_prompt->s->border_type);
	}

	FreeWidget(chat_widget, free_info);
	FreeWidget(input_prompt, free_prompt);
	tc_shutdown();
	return 0;
}

static void rerender_window(WINDOW *w, enum border_type bt)
{
	wclear(w);
	box(w, 0, 0);
	border_window(w, bt);
	wrefresh(w);
}

static void init()
{
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	start_color();
}

static void tc_shutdown()
{
	endwin();
}

static void print_fkeys()
{
	const char **list = fkeys_info;
	wmove(stdscr, 0, 0);
	while (*list) {
		attron(A_REVERSE);
		addstr(*list);
		attroff(A_REVERSE);
		wmove(stdscr, 0, getcurx(stdscr) + 1);
		list++;
	}
	wmove(stdscr, 0, 0);
}
