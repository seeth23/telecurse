#include "widget.h"

WINDOW *GPromptWidget(prompt_t *t)
{
	return NULL;
}

WINDOW *GInputWidget(input_t *t)
{
	return NULL;
}

WINDOW *GMenuWidget(menu_t *t)
{
	
	return NULL;
}

window_t *GInitSz(int height, int width, int y, int x)
{
	window_t *t = malloc(sizeof(window_t));
	t->height = height;
	t->width = width;
	t->starty = y;
	t->startx = x;
	return t;
}

void FreeWidget(void *widget, enum free_type t)
{
	switch(t) {
		case free_input:
			free(((input_t *)widget)->s);
			break;
		case free_menu:
			free(((menu_t *)widget)->s);
			break;
		case free_prompt:
			free(((prompt_t *)widget)->s);
			break;
	}
	free(widget);
}

