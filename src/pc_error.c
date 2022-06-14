#include "pc_error.h"

void error_panic(FILE *f, const char *fmt, ...)
{
	endwin();
	va_list vl;
	va_start(vl, fmt);
	vfprintf(f, fmt, vl);
	va_end(vl);
	exit(1);
}
