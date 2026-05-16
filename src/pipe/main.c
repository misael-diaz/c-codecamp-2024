#define _GNU_SOURCE
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

// server: sends to client via Unix socket the data passed through a pipe, think of the
// the pipe as another process that does an HTTP GET from an API endpoint via the curl
// command.
int main ()
{
	errno = 0;
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		_exit(1);
	}

	struct sockaddr_un sockaddr = {};
	struct sockaddr_un peeraddr = {};
	struct sockaddr_un *saddrp = &sockaddr;
	socklen_t peeraddrsz = sizeof(*saddrp);
	sockaddr.sun_family = AF_UNIX;
	uint64_t sunpathlen = (sizeof(sockaddr.sun_path) - 1);
	char sockpath[] = "sock";
	int bytes = snprintf(sockaddr.sun_path, sizeof(sockaddr.sun_path), "%s", sockpath);
	if (bytes >= ((int) sizeof(sockaddr.sun_path))) {
		sockaddr.sun_path[sunpathlen] = 0;
	}

	errno = 0;
	int64_t rc = bind(sockfd, (struct sockaddr*) &sockaddr, sizeof(*saddrp));
	if (-1 == rc) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		_exit(1);
	}

	errno = 0;
	int backlog = 32;
	rc = listen(sockfd, backlog);
	if (-1 == rc) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		remove(sockpath);
		_exit(1);
	}

	errno = 0;
	uint64_t const pagesz = sysconf(_SC_PAGE_SIZE);
	char cmd[] = "echo \"hello world\" | sed 's/world/socket/g'";
	FILE *p = popen(cmd, "r");
	if (!p) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		remove(sockpath);
		_exit(1);
	}

	errno = 0;
	uint64_t len_mmap = (pagesz << 1);
	void *dstbuf = mmap(NULL, len_mmap, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (MAP_FAILED == dstbuf) {
		fprintf(stderr, "%s\n", strerror(errno));
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		remove(sockpath);
		pclose(p);
		_exit(1);
	}

	rc = 0;
	int fd = fileno(p);
	uint64_t bytes_read = 0;
	do {
		errno = 0;
		rc = read(fd, dstbuf + bytes_read, pagesz);
		if (rc > 0) {
			bytes_read += rc;
			if ((len_mmap - bytes_read) <= pagesz) {
				dstbuf = mremap(dstbuf, len_mmap, (len_mmap << 1), MREMAP_MAYMOVE);
				if (MAP_FAILED == dstbuf) {
					if (errno) {
						fprintf(stderr, "%s\n", strerror(errno));
					}
					_exit(1);
				}
				len_mmap <<= 1;
			}
		}
		else if (-1 == rc) {
			if (errno) {
				fprintf(stderr, "%s\n", strerror(errno));
			}
			remove(sockpath);
			pclose(p);
			_exit(1);
		}
	} while (rc);

	fprintf(stdout, "%s\n", (char*) dstbuf);

	struct pollfd fds = {};
	fds.fd = sockfd;
	fds.events = (POLLIN | POLLOUT | POLLERR);
	int const nfds = 1;
	int const timeout = 32;
	int sw = 1;
	do {
		errno = 0;
		rc = poll(&fds, nfds, timeout);
		if (-1 == rc) {
			if (errno) {
				fprintf(stderr, "%s\n", strerror(errno));
			}
			remove(sockpath);
			pclose(p);
			_exit(1);
		}
		else if (rc) {

			fprintf(stdout, "%s", "event: client reading from socket\n");
			if (POLLIN == fds.revents) {

				errno = 0;
				int connfd = accept(
						sockfd,
						(struct sockaddr *) &peeraddr,
						&peeraddrsz
				);
				if (-1 == connfd) {
					if (errno) {
						fprintf(stderr, "%s\n", strerror(errno));
					}
					remove(sockpath);
					pclose(p);
					_exit(1);
				}

				int64_t bytes_written = 0;
				do {
					errno = 0;
					rc = write(
						connfd,
						dstbuf + bytes_written,
						pagesz - bytes_written
					);
					if (-1 == rc) {
						if (errno) {
							fprintf(
								stderr,
								"%s\n",
								strerror(errno)
							);
						}
						remove(sockpath);
						pclose(p);
						_exit(1);
					}
					else if (rc) {
						bytes_written += rc;
					}
				} while (rc);
			}
			break;
		} else {
			if (sw) {
				fprintf(stdout, "%s", "not available for writing\n");
				sw ^= 1;
			}
		}
	} while (1);

	pclose(p);
	errno = 0;
	rc = remove(sockpath);
	if (-1 == rc) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		_exit(1);
	}
	_exit(0);
	return 0;
}
