#include "history.h"

static char *history[MAX_HISTORY];
static size_t ipos = 0;

/* TODO make timeline for string in history! */

static void alloc_history(size_t len)
{
	if (history[ipos]) {
		free(history[ipos]);
		return;
	}
	history[ipos] = malloc(len);
	if (!history[ipos])
		error_panic(stderr, "Could not allocate memory for history pointer\n");
}

int save_history(const char *msg)
{
	if (!msg)
		return -1;
	if (ipos >= MAX_HISTORY-1)
		return -1;
	alloc_history(strlen(msg)+1);
	strcpy(history[ipos++], msg);
	return 0;
}

int remove_history()
{
	memset(history, 0, sizeof(history));
	ipos = 0;
	return 0;
}

char **get_history()
{
	history[ipos] = NULL;
	return history;
}

void free_history()
{
	while (ipos > 0) {
		if (history[ipos])
			free(history[ipos]);
		ipos--;
	}
	ipos = 0;
}

