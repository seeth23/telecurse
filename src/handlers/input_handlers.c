#include "input_handlers.h"
#include "menu_handlers.h"

#include "../misc.h"

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

void handle_function_keys(int ch)
{
	switch (ch) {
		case KEY_F(5): {
			const char *opt[] = { "Yes", "No", NULL };
			size_t opt_size = BLK_SIZE(opt);
			menu_t *menu_widget = GMenuWidget(opt, "Exit?", opt_size, 2, 20, 1, 1, border_default);
			menu_widget->choose(menu_widget, test_handle_menu);
			FreeWidget(menu_widget, free_menu);
			break;
			endwin();
		}
	}
}
