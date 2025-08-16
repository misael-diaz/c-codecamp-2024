#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PORT 8000 
#define LISTEN_BACKLOG 40
#define BUFSZ 1024

// implements a tcp listener that just listens on port `PORT` on the loopback-device 
int main()
{
	char buf[BUFSZ];
	memset(buf, 0, sizeof(buf));
	errno = 0;
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
		.sin_addr = {
			htonl(INADDR_LOOPBACK)
		}
	};
	struct sockaddr_in peer_addr = {};
	socklen_t peer_addr_size = sizeof(sin);
	int tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if ((-1 == tcp_socket_fd) || errno) {
		if (errno) {
			fprintf(stderr, "main: %s\n", strerror(errno));
		} else {
			fprintf(stderr, "main: %s\n", "OpenSocketError");
		}
		exit(EXIT_FAILURE);
	}

	errno = 0;
	int rc = bind(tcp_socket_fd, (struct sockaddr*) &sin, sizeof(sin));
	if ((-1 == rc) || errno) {
		if (errno) {
			fprintf(stderr, "main: %s\n", strerror(errno));
		} else {
			fprintf(stderr, "main: %s\n", "BindError");
		}
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	rc = listen(tcp_socket_fd, LISTEN_BACKLOG);
	if ((-1 == rc) || errno) {
		if (errno) {
			fprintf(stderr, "main: %s\n", strerror(errno));
		} else {
			fprintf(stderr, "main: %s\n", "ListenError");
		}
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	errno = 0;
	int cfd = accept(tcp_socket_fd, (struct sockaddr*) &peer_addr, &peer_addr_size);
	if ((-1 == cfd) || errno) {
		if (errno) {
			fprintf(stderr, "main: %s\n", strerror(errno));
		} else {
			fprintf(stderr, "main: %s\n", "AcceptError");
		}
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	errno = 0;
	ssize_t bytes = read(cfd, buf, BUFSZ);
	if (-1 == bytes || errno) {
		if (errno) {
			fprintf(stderr, "main: %s\n", strerror(errno));
		} else {
			fprintf(stderr, "main: %s\n", "ReadError");
		}
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "%s\n", buf);

	close(tcp_socket_fd);
	return 0;
}
