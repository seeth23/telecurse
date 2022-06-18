#include "input_handlers.h"

#include "../widget.h"
#include "../math/center.h"
#include "../history.h"
#include "../types.h"
#include "../history.h"

static void test_handle_menu(int i)
{
	switch (i) {
		case 0:
			endwin();
			free_history();
			exit(0);
		case 1:
			break;
	}
}
enum {
	MENU_HEIGHT = 2,
	MENU_WIDTH  = 10,
};

static void write_history(info_t *t, const char *msg, const char *null)
{
	t->msg_num++;
	scrollok(t->w, TRUE);
	wmove(t->w, t->current_y++, t->current_x);
	wprintw(t->w, "%s\n ", msg);
	border_window(t->w, t->s->border_type);
	wrefresh(t->w);
}

void handle_function_keys(int ch)
{
	const int maxy = stdscr->_maxy;
	const int maxx = stdscr->_maxx;

	switch (ch) {
		case KEY_F(1): {
			//centercords_t help_cords;
			break;
		}
		case KEY_F(2):{
			//centercords_t find_cords;
			break;
		}
		case KEY_F(3): {
			//centercords_t users_cords;
			break;
		}
		case KEY_F(4): {
			const int height = maxy/2;
			const int width = maxx/2;
			centercords_t history_cords;
			int ch;
			center_cord(height, width, &history_cords.y, &history_cords.x);
			info_t *history_widget = GInfoWidget("History",
					height,
					width,
					history_cords.y,
					history_cords.x,
					border_default);

			history_widget->write = write_history;
			char **ptr = get_history();
			while (*ptr)
				history_widget->write(history_widget, *ptr++, NULL);
			while ((ch = wgetch(history_widget->w))) {
				switch (ch) {
					case 'q':
					case 'Q':
						goto exit_while_label;
						break;
				}
			}
exit_while_label:
			FreeWidget(history_widget, free_info);
			break;
		}
		case KEY_F(5): {
			centercords_t quit_cords;
			center_cord(MENU_HEIGHT, MENU_WIDTH, &quit_cords.y, &quit_cords.x);
			const char *opt[] = { "Yes", "No", NULL };
			size_t opt_size = BLK_SIZE(opt);
			menu_t *menu_widget = GMenuWidget(opt, "Exit?", opt_size, MENU_HEIGHT, MENU_WIDTH, quit_cords.y, quit_cords.x, border_default);
			menu_widget->choose(menu_widget, test_handle_menu);
			FreeWidget(menu_widget, free_menu);
			free_history();
			break;
		}
	}
}
