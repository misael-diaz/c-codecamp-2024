#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_TOKEN_SIZE 80

ssize_t parse(
		char const ** const iter,
		char const ** const beg,
		char const ** const end,
		char const delim
)
{
	ssize_t count = 0;
	*beg = *iter;
	while ((**iter) && (delim != **iter)) {
		++*iter;
	}
	if (!**iter) {
		count = -1;
		fprintf(stderr, "parse: %s\n", "FormatCSVError");
	} else {
		++*iter;
		*end = *iter;
		count = (*end - *beg);
		if ('\n' == delim && **iter) {
			count = -1;
			fprintf(stderr, "parse: %s\n", "TrailCSVError");
		}
	}
	return count;
}

ssize_t tokenize(
		char *token,
		char const *beg,
		char const *end,
		char const delim
)
{
	ssize_t const bytes = (end - beg);
	ssize_t const len = (bytes - 1);
	if (
		(0 >= bytes) ||
		(MAX_TOKEN_SIZE < bytes)
	   ) {
		fprintf(stderr, "tokenize: %s\n", "TokenSizeError");
		return -1;
	}
	memset(token, 0, MAX_TOKEN_SIZE);
	memcpy(token, beg, bytes);
	if (delim != token[len]) {
		fprintf(stderr, "tokenize: %s\n", "TokenDelimeterError");
		return -1;
	}
	token[len] = 0;
	return len;
}

int main()
{
	char tokens[3][MAX_TOKEN_SIZE];
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
			char const *beg = NULL;
			char const *end = NULL;
			fprintf(stdout, "%s", lineptr);
			if (-1 == parse(&iter, &beg, &end, ',')) {
				goto err;
			}
			if (-1 == tokenize(tokens[0], beg, end, ',')) {
				goto err;
			}
			fprintf(stdout, "token: %s\n", tokens[0]);
			if (-1 == parse(&iter, &beg, &end, ',')) {
				goto err;
			}
			if (-1 == tokenize(tokens[1], beg, end, ',')) {
				goto err;
			}
			fprintf(stdout, "token: %s\n", tokens[1]);
			if (-1 == parse(&iter, &beg, &end, '\n')) {
				goto err;
			}
			if (-1 == tokenize(tokens[2], beg, end, '\n')) {
				goto err;
			}
			fprintf(stdout, "token: %s\n", tokens[2]);
		}
	} while (sw);
	if (lineptr) {
		free(lineptr);
		lineptr = NULL;
		n = 0;
	}
	fclose(file);
	return 0;
err:
	{
		if (lineptr) {
			free(lineptr);
			lineptr = NULL;
			n = 0;
		}
		fclose(file);
		exit(EXIT_FAILURE);
	}
}
