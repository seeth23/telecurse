#include "client.h"

int
client_init(const char *ipaddr, int port)
{
	int fd, res;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(ipaddr, &addr.sin_addr);

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == sock_err) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	res = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (res == sock_err) {
		perror("connect()");
		exit(EXIT_FAILURE);
	}
	return fd;
}

int
listen_server(int fd, char *buffer, int buf_len)
{
	fd_set readfd;
	int max_d = fd;
	int res;
	int read_r;

	FD_ZERO(&readfd);

	FD_SET(fd, &readfd);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 2000;

	res = select(max_d+1, &readfd, NULL, NULL, &timeout);

	if (res == select_err) {
		if (errno == EINTR) {
			return select_err;
		}
		return res;
	} else if (res == select_timeout) {
		return select_timeout;
	}

	if(FD_ISSET(fd, &readfd)) {
		read_r = read(fd, buffer, buf_len);
	}

	return read_r;
}

int
write_server(int fd, const char *str)
{
	int written;
	written = write(fd, str, strlen(str));
	return written;
}
