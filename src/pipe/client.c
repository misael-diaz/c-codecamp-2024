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

int main ()
{
	errno = 0;
	int64_t rc = 0;
	uint64_t const pagesz = sysconf(_SC_PAGE_SIZE);
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		_exit(1);
	}

	struct sockaddr_un addr = {};
	struct sockaddr_un *addrp = &addr;
	socklen_t const addrsz = sizeof(*addrp);
	addr.sun_family = AF_UNIX;
	char sockpath[] = "sock";
	int bytes = snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", sockpath);
	if (bytes >= ((int) sizeof(addr.sun_path))) {
		fprintf(stderr, "%s\n", "error: sun path truncation");
		_exit(1);
	}

	errno = 0;
	rc = connect(sockfd, (struct sockaddr*) &addr, addrsz);
	if (-1 == rc) {
		if (errno) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
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
		_exit(1);
	}

	rc = 0;
	uint64_t bytes_read = 0;
	do {
		errno = 0;
		rc = read(sockfd, dstbuf + bytes_read, pagesz);
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
			_exit(1);
		}
	} while (rc);

	fprintf(stdout, "%s\n", (char*) dstbuf);
	_exit(0);
	return 0;
}
