#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "pc_error.h"
#include "tc_window.h"
#include "types.h"
#include "input.h"
#include "widget.h"
#include "misc.h"
#include "math/center.h"
#include "history.h"
#include "client.h"
#include "args.h"

enum {
	NAME_PROMPT_HEIGHT = 4,
	NAME_PROMPT_WIDTH  = 20,
	BUF_LEN = 2048,
	MAX_MSGS_PER_REQUEST = 40,
};

static const char *PROGNAME = "tc";

static void rerender_window(WINDOW *w, enum border_type);
static void print_fkeys();
static void tc_shutdown();
static void init();
static void usage();
static size_t messages_count(const char *str, char buffers[MAX_MSGS_PER_REQUEST][BUF_LEN]);
static char *format_message_history(const char *msg);

/* must end up with NULL for easier looping */
static const char *fkeys_info[] = {"F1 - Help", "F2 - Find",  "F3 - Users", "F4 - History", "F5 - Exit", NULL};

int
main(int argc, char **argv)
{
	if (argc != 3) {
		usage();
	}
	opt_t opt;
	args_s(argv, &opt);
	int client_socket = client_init(opt.ipaddr, opt.port);

	init();
	print_fkeys();

	char client_name[20];
	centercords_t nameprompt_cords;
	centercords_t chat_cords;
	centercords_t input_cords;

	const int CHAT_HEIGHT = stdscr->_maxy/2;
	const int CHAT_WIDTH = stdscr->_maxx/2;
	const int INPUT_CHAT_HEIGHT = 3;
	const int INPUT_CHAT_WIDTH = stdscr->_maxx/2;

	center_cord(
			NAME_PROMPT_HEIGHT,
			NAME_PROMPT_WIDTH,
			&nameprompt_cords.y,
			&nameprompt_cords.x);

	center_cord(
			CHAT_HEIGHT,
			CHAT_WIDTH,
			&chat_cords.y,
			&chat_cords.x);

	center_cord(
			INPUT_CHAT_HEIGHT,
			INPUT_CHAT_WIDTH,
			&input_cords.y,
			&input_cords.x);

	prompt_t *name_prompt_widget = GPromptWidget(
			"Enter name",
			18,
			NAME_PROMPT_HEIGHT,
			NAME_PROMPT_WIDTH,
			nameprompt_cords.y,
			nameprompt_cords.x,
			border_type2);

	name_prompt_widget->read(name_prompt_widget);
	strcpy(client_name, name_prompt_widget->input);
	FreeWidget(name_prompt_widget, free_prompt);

	info_t *chat_widget = GInfoWidget(
			"Chat",
			CHAT_HEIGHT,
			CHAT_WIDTH,
			chat_cords.y,
			chat_cords.x,
			border_default);

	prompt_t *input_prompt = GPromptWidget(
			NULL,
			255,
			INPUT_CHAT_HEIGHT,
			INPUT_CHAT_WIDTH,
			input_cords.y+CHAT_HEIGHT/2+(INPUT_CHAT_HEIGHT/2+1),
			input_cords.x,
			border_type2);

	char SERVER_ANSWER[BUF_LEN];
	wtimeout(input_prompt->w, 10);
	for (;;) {
		memset(SERVER_ANSWER, 0, sizeof(SERVER_ANSWER));
		char fmt_message_name[BUF_LEN]; /* in this variable I will use sprintf(fmt_message_name, "%s: %s", ->input, name). Then it will be send to server. */
		input_prompt->read(input_prompt);
		if (!input_prompt->input) {
			int read_server;
			read_server = listen_server(client_socket, SERVER_ANSWER, BUF_LEN);
			if (read_server > 0) {

				char messages[MAX_MSGS_PER_REQUEST][BUF_LEN];
				size_t lines = messages_count(SERVER_ANSWER, messages);

				for (size_t i = 0; i < lines; i++) {
					chat_widget->write(chat_widget, messages[i]);

					char *tmp = format_message_history(messages[i]);
					save_history(tmp);
					free(tmp);
				}
			}
			continue;
		}
		if (input_prompt->input) {
			sprintf(fmt_message_name, "%s: %s", client_name, input_prompt->input);
		}
		write_server(client_socket, fmt_message_name);

		if (input_prompt->input)
			free(input_prompt->input);
		rerender_window(input_prompt->w, input_prompt->s->border_type);
	}

	/* Unreachable code. when F5 is pressed it uses its own handlers for exit.
	 * Maybe I should change some `global` variable, so that can access to multiply files(its very unpretty imo).
	 * So I can change condition in 'forever' loop like CHAT_RUNNIG = 0, so code reaches this section. Need to think about it.
	 * P.S. by the way i will definetily work because of wtimeout and (!input_prompt->input) condition with continue. Maybe that's the case */

	free_history();
	FreeWidget(chat_widget, free_info);
	FreeWidget(input_prompt, free_prompt);
	tc_shutdown();
	return 0;
}

static void
rerender_window(WINDOW *w, enum border_type bt)
{
	wclear(w);
	box(w, 0, 0);
	border_window(w, bt);
	wrefresh(w);
}

static void
init()
{
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	start_color();
	zero_history();
}

static void
tc_shutdown()
{
	endwin();
}

static void
print_fkeys()
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

static void
usage()
{
	fprintf(stderr, "usage:  %s <ip> <port>\n", PROGNAME);
	fprintf(stderr, "\tport <= 65535\n");
	exit(EXIT_FAILURE);
}

static size_t
messages_count(const char *str, char buffers[MAX_MSGS_PER_REQUEST][BUF_LEN])
{
	const char *ptr = str;
	memset(buffers, 0, MAX_MSGS_PER_REQUEST*BUF_LEN);
	size_t count = 0, i, j;
	while (*ptr) {
		if (*ptr == '\n') {
			count++;
		}
		ptr++;
	}
	ptr = str;
	i = 0;
	j = 0;
	while (*ptr) {
		if (*ptr == '\n') {
			buffers[i][j] = 0;
			i++;
			j = 0;
		} else {
			buffers[i][j] = *ptr;
			j++;
		}
		ptr++;
	}
	return count;
}

static char *
format_message_history(const char *msg)
{
	if (msg == NULL)
		error_panic(stderr, "Source msg is NULL. Can't format\n");
	char *dest = malloc(strlen(msg));
	if (!dest)
		error_panic(stderr, "Could not allocate memory for history buffer\n");
	strcpy(dest, msg);
	return dest;
}
