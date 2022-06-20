#include "misc.h"

//static const char *days[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

char *cur_date()
{
	time_t t = time(NULL);
	struct tm *time = localtime(&t);
	char *fmt_time = malloc(40*sizeof(char));
	//sprintf(fmt_time, "%02d-%02d-%d %02d:%02d:%02d", time->tm_mday, (time->tm_mon)+1, time->tm_year+1900, time->tm_hour, time->tm_min, time->tm_sec);
	sprintf(fmt_time, "%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
	return fmt_time;
}
