#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "pc_error.h"
#include "net_types.h"
#include "history.h"

int client_init(const char *ipaddr, int port);
int listen_server(int fd, char *buffer, int buf_len);
int write_server(int fd, const char *str);

#endif
