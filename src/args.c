#include "args.h"

void
args_s(char **argv, opt_t *t)
{
	argv++;
	const char *ipaddr = argv[0];
	const char *port = argv[1];

	regex_t re;

	if (regcomp(&re, "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$", REG_NOSUB|REG_EXTENDED) != 0)
		error_panic(stderr, "Failed to init regex\n");

	if (regexec(&re, ipaddr, (size_t) 0, NULL, 0) != 0)
		error_panic(stderr, "Failed at regex\n");

	t->ipaddr = ipaddr;
	t->port = atoi(port);
	if (t->port <= 0 || t->port > 65535)
		error_panic(stderr, "Wrong port\n");
}
