#ifndef __NET_TYPES_H__
#define __NET_TYPES_H__

enum consts {
	max_clients    = 64,
	buf_len        = 512,
	wait_time_sec  = 2,
	wait_time_msec = 500,
	max_queue      = 8,
	name_len       = 20,
};

enum rets {
	sock_err = -1,
	select_err      = -1,
	invalid_socket = 0,
	select_timeout = 0,
};

#endif
