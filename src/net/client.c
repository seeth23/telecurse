#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

enum {
	buf_len = 255,
};

int main()
{
	int clfd, res;
	struct sockaddr_in c_addr;
	c_addr.sin_port = htons(7654);
	c_addr.sin_family = AF_INET;
	res = inet_pton(AF_INET, "127.0.0.1", &c_addr.sin_addr);

	if (!res) {
		perror("inet_pton():");
		exit(1);
	}

	clfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clfd == -1) {
		perror("socket()");
		exit(1);
	}

	res = connect(clfd, (struct sockaddr *)&c_addr, sizeof(c_addr));
	if (res == -1) {
		perror("connect()");
		exit(1);
	}
	
#if 0
	int pid = fork();
	if (pid == 0) { /* listening child */
		int readd;
		char buf[buf_len];
		while ((readd = read(clfd, buf, buf_len)) > 0) {
			write(1, buf, readd);
			memset(buf, 0, buf_len);
		}
		printf("child's shutting down...\n");
		shutdown(clfd, SHUT_RDWR);
		close(clfd);
		exit(0);
	}

#endif
	char buf[buf_len];
	memset(buf, 0, sizeof(buf));
	for (;;) {
		int res;
		fd_set readfds;
		fd_set writefds;

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);


		FD_SET(0, &readfds);
		FD_SET(clfd, &readfds);
		if (buf[0]!=0)
			FD_SET(clfd, &writefds);

		//max_d = clfd;

		res = select(clfd+1, &readfds, &writefds, NULL, NULL);

		if (res == -1) {
			if (errno == EINTR) {
				//perror("EINTR");
				continue;
			}
			perror("select()");
			continue;
		} else if (res == 0) {
			printf("timeout expired\n");
			continue;
		}
		int rread;

		if (FD_ISSET(0, &readfds)) {
			rread = read(0, buf, buf_len);
			if (rread == 0) {
				shutdown(clfd, SHUT_RDWR);
				close(clfd);
				exit(0);
			}
		}

		if (FD_ISSET(clfd, &readfds)) {
			char buf_from_serv[buf_len];
			memset(buf_from_serv, 0, sizeof(buf_from_serv));
			rread = read(clfd, buf_from_serv, buf_len);
			if (rread == 0) {
				shutdown(clfd, SHUT_RDWR);
				close(clfd);
				exit(0);
			}
			printf("%s", buf_from_serv);
		}

		if (FD_ISSET(clfd, &writefds)) {
			write(clfd, buf, rread);
			memset(buf, 0, sizeof(buf));
			rread = 0;
		}

	}

	return 0;
}
