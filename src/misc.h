#ifndef __MISC_H__
#define __MISC_H__

#define BLK_SIZE(stn) (sizeof(stn)/sizeof(stn[0]))
#define BASE_MENU_HEIGHT 2
#define ctrl(x) ((x) & 0x1f)

char *cur_date();

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#endif
