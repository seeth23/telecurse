#ifndef __TC_ERROR_H__
#define __TC_ERROR_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void error_panic(FILE *f, const char *fmt, ...);

#endif
