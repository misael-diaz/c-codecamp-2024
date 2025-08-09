#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

ssize_t parse(
		char const ** const iter,
		char const delim
)
{
	ssize_t count = 0;
	char const *beg = *iter;
	while ((**iter) && (delim != **iter)) {
		++*iter;
	}
	char const *end = *iter;
	if (!**iter) {
		count = -1;
	} else {
		count = (end - beg);
		++*iter;
	}
	return count;
}

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
			char const *iter = lineptr;
			fprintf(stdout, "%s", lineptr);
			fprintf(stdout, "count: %ld\n", parse(&iter, ','));
			fprintf(stdout, "count: %ld\n", parse(&iter, ','));
			fprintf(stdout, "count: %ld\n", parse(&iter, '\n'));
		}
	} while (sw);
	if (lineptr) {
		free(lineptr);
		lineptr = NULL;
		n = 0;
	}
	fclose(file);
	return 0;
}
