#include "input_handlers.h"

#include "../widget.h"
#include "../math/center.h"

static void test_handle_menu(int i)
{
	switch (i) {
		case 0:
			endwin();
			exit(0);
			break;
		case 1:
			break;
	}
}
enum {
	MENU_HEIGHT = 2,
	MENU_WIDTH  = 10,
};

void handle_function_keys(int ch)
{
	switch (ch) {
		case KEY_F(5): {
			int starty, startx;
			center_cord(MENU_HEIGHT, MENU_WIDTH, &starty, &startx);
			const char *opt[] = { "Yes", "No", NULL };
			size_t opt_size = BLK_SIZE(opt);
			menu_t *menu_widget = GMenuWidget(opt, "Exit?", opt_size, MENU_HEIGHT, MENU_WIDTH, starty, startx, border_default);
			menu_widget->choose(menu_widget, test_handle_menu);
			FreeWidget(menu_widget, free_menu);
			break;
		}
	}
}
