#include "history.h"

static char *history[MAX_HISTORY];
static ssize_t ipos = 0;

void zero_history()
{
	int i;
	for (i = 0; i < MAX_HISTORY; i++) {
		history[i] = NULL;
	}
}

static void alloc_history(size_t len)
{
	/*if (history[ipos]) {
		free(history[ipos]);
		return;
	}*/
	history[ipos] = malloc(len);
	if (!history[ipos]) error_panic(stderr, "Could not allocate memory for history pointer\n");
}

int history_size()
{
	return ipos-1;
}

int save_history(const char *msg)
{
	if (!msg)
		return -1;
	if (ipos >= MAX_HISTORY-1)
		return -1;
	alloc_history(strlen(msg)+1);
	strcpy(history[ipos], msg);
	ipos++;
	return 0;
}

char **get_history()
{
	return history;
}

void free_history()
{
	while (ipos >= 0) {
		if (history[ipos])
			free(history[ipos]);
		ipos--;
	}
	ipos = 0;
}

