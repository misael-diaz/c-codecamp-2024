#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PORT 8000
#define LISTEN_BACKLOG 40
#define BUFSZ 1024
#define REQUEST_LINE_SIZE 256

#define HTTP_VERSION "1.1"
#define REQUEST_TARGET "/"
#define METHOD "GET"

#define CR '\r'
#define LF '\n'

struct RequestLine {
	char const *version;
	char const *target;
	char const *method;
};

struct Request {
	struct RequestLine request_line;
};

/*
   struct Request req = {
	   .request_line = {
		   .version = HTTP_VERSION,
		   .target = REQUEST_TARGET,
		   .method = METHOD
	   }
   };
*/

void checkRequestLine(struct Request * const req)
{
	assert(0 == strcmp(HTTP_VERSION, req->request_line.version));
	assert(0 == strcmp(REQUEST_TARGET, req->request_line.target));
	assert(0 == strcmp(METHOD, req->request_line.method));
}

ssize_t parseRequestLine(
	char const ** const buf,
	char const ** const beg,
	char const ** const end
)
{
	int sw = 1;
	ssize_t len = 0;
	char const *it = *buf;
	*beg = it;
	*end = it;
	do {
		while (*it && (CR != *it)) {
			++it;
		}
		if (!*it) {
			sw = 0;
			fprintf(stderr,
				"parseRequestHeader: %s\n",
				"NullByteError");
			return -1;
		} else if (CR == *it) {
			++it;
			if (!*it) {
				sw = 0;
				fprintf(stderr,
					"parseRequestHeader: %s\n",
					"NullByteError");
				return -1;
			} else if (LF == *it) {
				sw = 0;
				++it;
				*end = it;
				break;
			}
		}
	} while (sw);

	len = (*end - *beg);
	if (0 >= len) {
		fprintf(stderr,
			"parseRequestHeader: %s\n",
			"RequestLineLengthError");
		return -1;
	}

	*buf = *end;
	return len;
}

int main()
{
	char buf[BUFSZ];
	char rl[REQUEST_LINE_SIZE];
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
	char const *beg = buf;
	char const *end = buf;
	char const *b = buf;
	ssize_t len = parseRequestLine(&b, &beg, &end);
	if (-1 == len) {
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	ssize_t request_line_bytes = (1 + len);
	if (REQUEST_LINE_SIZE < request_line_bytes) {
		fprintf(stderr, "main: %s\n", "RequestLineBufferWouldOverflow");
		close(tcp_socket_fd);
		exit(EXIT_FAILURE);
	}

	memset(rl, 0, REQUEST_LINE_SIZE);
	memcpy(rl, beg, len);
	rl[len] = 0;

	fprintf(stdout, "main: request-line: %s\n", rl);

	close(tcp_socket_fd);
	return 0;
}
