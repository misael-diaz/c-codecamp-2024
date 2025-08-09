#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
	errno = 0;
	FILE *file = fopen("problems.csv", "r");
	if (!file) {
		fprintf(stderr, "main: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char *lineptr = NULL;
	size_t n = 0;
	int sw = 1;
	do {
		errno = 0;
		ssize_t const bytes = getline(&lineptr, &n, file);
		if (-1 == bytes) {
			sw = 0;
			if (errno) {
				fprintf(stderr, "main: %s\n", strerror(errno));
				if (lineptr) {
					free(lineptr);
					lineptr = NULL;
					n = 0;
				}
				exit(EXIT_FAILURE);
			}
			fprintf(stdout, "%s\n", "EOF");
		} else {
			fprintf(stdout, "%s", lineptr);
		}
	} while (sw);
	free(lineptr);
	lineptr = NULL;
	n = 0;
	return 0;
}
