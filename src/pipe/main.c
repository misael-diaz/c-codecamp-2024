#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

int main ()
{
	errno = 0;
	uint64_t const pagesz = sysconf(_SC_PAGE_SIZE);
	char cmd[] = "echo \"hello world\" | sed 's/world/pipe/g'";
	FILE *p = popen(cmd, "r");
	if (!p) {
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
		pclose(p);
		_exit(1);
	}

	int64_t rc = 0;
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
			pclose(p);
			_exit(1);
		}
	} while (rc);

	fprintf(stdout, "%s\n", (char*) dstbuf);

	pclose(p);
	return 0;
}
