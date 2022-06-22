#include "input_handlers.h"

#include "../widget.h"
#include "../math/center.h"
#include "../history.h"
#include "../types.h"
#include "../history.h"

static void quit_menu_handle(int i)
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

static void clear_lines(info_t *t)
{
	int i, j;
	for (i = 1; i < t->s->height-1; i++) {
		for (j = 1; j < t->s->width-1;j++) {
			mvwaddch(t->w, i, j, ' ');
		}
	}
}

static void write_page_history(info_t *t, const char *msg)
{
	t->msg_num++;
	mvwprintw(t->w, 0, 1, "%s", t->title);
	mvwprintw(t->w, t->current_y++, 1, "%s", msg);
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
		case KEY_F(2): {
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

			history_widget->write = write_page_history;
			char **ptr = get_history();

			size_t history_page_current = 0;
			size_t history_page_step = history_widget->s->height-2;

			for (int i = 0; i < history_page_step && *(ptr+i); i++) {
				history_widget->write(history_widget, *(ptr+i));
			}

			while ((ch = wgetch(history_widget->w))) {
				switch (ch) {
					case 'q':
					case 'Q':
						goto exit_while_label;
						break;
					case KEY_DOWN:
					case 'j': { /* down */
						int PAGECOUNT = (double)history_size() / (double)history_page_step;
						history_widget->current_y = 1;
						if (history_page_current < PAGECOUNT) {
							history_page_current++;
							int new_step = history_page_step * history_page_current;
							clear_lines(history_widget);
							for (int i = 0; i < history_page_step && *(ptr+i+new_step); i++) {
								history_widget->write(history_widget, *(ptr+i+new_step));
							}
						}
					}
						break;
					case KEY_UP:
					case 'k': /* up */
						history_widget->current_y = 1;
						if (history_page_current>0) {
							history_page_current--;
							int new_step = history_page_step*history_page_current;
							clear_lines(history_widget);
							for (int i = 0; i < history_page_step && *(ptr+i+new_step); i++) {
								history_widget->write(history_widget, *(ptr+i+new_step));
							}
						}
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
			menu_widget->choose(menu_widget, quit_menu_handle);
			FreeWidget(menu_widget, free_menu);
			free_history();
			break;
		}
	}
}
