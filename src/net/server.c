#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../net_types.h"

#define TRUE 1
#define FALSE 0

struct clients_d {
	int32_t fds[max_clients];
	char *buffer[max_clients];
	int32_t fd_count;
	char name[max_clients][name_len];
};

volatile static sig_atomic_t server_running = TRUE;

void sigint_handler(int sig)
{
	server_running = FALSE;
	return;
}

static int32_t max_fd(struct clients_d *t);
static void shutdown_clients(struct clients_d *t);
static void rem_fd(struct clients_d *t, int32_t fd);
static void add_fd(struct clients_d *t, int32_t fd);
static struct clients_d init_clients_d();
static int32_t free_buffer(void *buf);
static void parse_name(const char *str, struct clients_d *t, int fd);

int main(int argc, char **argv)
{
	signal(SIGINT, sigint_handler);

	if (argc != 2) {
		fprintf(stderr, "usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int res;

	if (atoi(argv[1]) > 65535 || atoi(argv[1]) <= 0) {
		fprintf(stderr, "Invalid port\n");
		exit(EXIT_FAILURE);
	}

	u_int16_t port = atoi(argv[1]);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int32_t listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenfd == sock_err) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	int32_t opt = 1;
	res = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (res == sock_err) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	res = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
	if (res == sock_err) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	res = listen(listenfd, max_queue);
	if (res == sock_err) {
		perror("listen()");
		exit(EXIT_FAILURE);
	}

	/* struct for client descriptors and clients count */
	struct clients_d client_fds = init_clients_d();

	while (server_running) {
		char server_buffer[buf_len];
		memset(server_buffer, 0, sizeof(server_buffer));
		int32_t max_d, read_res;
		uint32_t i, fd;

		fd_set readfds;
		fd_set writefds;

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);

		FD_SET(listenfd, &readfds); /* listening fd always must be set */

		max_d = client_fds.fd_count > 0 ? max_fd(&client_fds) : listenfd;

		if (client_fds.fd_count > 0)
			for (i = 0, fd = client_fds.fds[i]; i < max_d+1; ++i, fd = client_fds.fds[i]) {
				if (fd == invalid_socket)
					continue;
				FD_SET(fd, &readfds);
				if (client_fds.buffer[fd])
					FD_SET(fd, &writefds);
			}

		res = select(max_d+1, &readfds, &writefds, NULL, NULL);
		/* handle return value of select() */
		if (res == select_err) {
			if (errno == EINTR) {
				//perror("EINTR");
				continue;
			}
			perror("select()");
			continue;
		} else if (res == select_timeout) {
			printf("timeout expired\n");
			continue;
		}
#if DEBUG
		else
			printf("%d descriptors were affected\n", res);
#endif

		if (FD_ISSET(listenfd, &readfds)) { /* listenfd is ready to accept new client */
			fd = accept(listenfd, NULL, NULL);
			add_fd(&client_fds, fd);
		}

		/* this loop can be a part of a function with callback */
		/* loop for checking every client socket for ready IO */
		if (client_fds.fd_count > 0) {
			for (i = 0, fd = client_fds.fds[i]; i < max_d+1; ++i, fd = client_fds.fds[i]) {
				if (fd == invalid_socket)
					continue;
				if (FD_ISSET(fd, &readfds)) { /* client answer is ready to be handled */
					read_res = read(fd, server_buffer, sizeof(server_buffer));
					server_buffer[read_res] = 0;
					if (read_res == 0) { /* eof --> client disconnected */
						FD_CLR(fd, &readfds);
						rem_fd(&client_fds, fd);
						continue;
					} else if (read_res == -1) {
						perror("read()");
						continue;
					}

					if (free_buffer(client_fds.buffer[fd])) {
						client_fds.buffer[fd] = NULL;
					}

					client_fds.buffer[fd] = malloc(read_res);
					if (!client_fds.buffer[fd]) {
						fprintf(stderr, "Failed to alloc client buffer\n");
						continue;
					}

					parse_name(server_buffer, &client_fds, fd);
					sprintf(client_fds.buffer[fd], "%s", server_buffer);
					memset(server_buffer, 0, sizeof(server_buffer));
				}

				if (FD_ISSET(fd, &writefds)) {
					for (int j = 0; j < max_d+1; j++) {
						if (client_fds.fds[j] == invalid_socket)
							continue;
						write(client_fds.fds[j], client_fds.buffer[fd], strlen(client_fds.buffer[fd]));
					}

					if (free_buffer(client_fds.buffer[fd])) {
						client_fds.buffer[fd] = NULL;
					}
				}
			}
		}
	}
	shutdown_clients(&client_fds);
	printf("ret\n");
	return 0;
}

static void shutdown_clients(struct clients_d *t)
{
	int32_t max_d = max_fd(t);
	int32_t i, fd;
	for (i = 0, fd = t->fds[i]; i < max_d+1; ++i, fd = t->fds[i]) {
		if (fd == invalid_socket)
			continue;

		if (free_buffer(t->buffer[fd]))
			t->buffer[fd] = NULL;

		shutdown(fd, SHUT_RDWR);
		close(fd);
	}
}

static int32_t max_fd(struct clients_d *t)
{
	int32_t i;
	int32_t max = 0;
	for (i=0; i < max_clients; i++) {
		if (t->fds[i] == 0)
			continue;
		if (t->fds[i] > max)
			max = t->fds[i];
	}
	return max;
}

static void rem_fd(struct clients_d *t, int32_t fd)
{
	t->fds[fd] = 0;
	t->fd_count--;
	printf("%s disconnected\n", t->name[fd]);

	if (free_buffer(t->buffer[fd]))
		t->buffer[fd] = NULL;
	
	shutdown(fd, SHUT_RDWR);
	close(fd);
}

static void add_fd(struct clients_d *t, int32_t fd)
{
	t->fds[fd] = fd;
	t->fd_count++;
	printf("%d connected\n", fd);
}

static struct clients_d init_clients_d()
{
	struct clients_d t;
	memset(t.fds, 0, sizeof(t.fds));
	memset(t.name, 0, sizeof(t.name));

	/* setting all pointers to client's buffer to NULL */
	for (int i = 0; i < max_clients; i++) {
		t.buffer[i] = NULL;
	}

	t.fd_count = 0;
	return t;
}

static int32_t free_buffer(void *buf)
{
	if (buf) {
		free(buf);
		return 1;
	}
	return 0;
}

static void parse_name(const char *str, struct clients_d *t, int fd)
{
	int i = 0;
	while (*str != ':') {
		t->name[fd][i] = *str++;
		i++;
	}
	t->name[fd][i] = 0;
}
