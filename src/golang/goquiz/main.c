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

ssize_t trim(
	char ** const iter,
	char ** const beg,
	char ** const end
)
{
	while ((**iter) && (' ' >= **iter)) {
		++*iter;
	}

	if (!**iter) {
		return -1;
	}

	*beg = *iter;
	while ((**iter) && (' ' < **iter) && ('~' >= **iter)) {
		++*iter;
	}

	if (!**iter) {
		return -1;
	}

	**iter = 0;
	*end = *iter;
	return 0;
}

int main()
{
	char tokens[3][MAX_TOKEN_SIZE];
	char word[MAX_TOKEN_SIZE];
	errno = 0;
	FILE *file = fopen("problems.csv", "r");
	if (!file) {
		fprintf(stderr, "main: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	char const *type = NULL;
	char const *question = NULL;
	char const *answer = NULL;
	char *lineptr = NULL;
	char *textptr = NULL;
	size_t n = 0;
	size_t m = 0;
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
			break;
		}
		char const *iter = lineptr;
		char const *beg = NULL;
		char const *end = NULL;
		if (-1 == parse(&iter, &beg, &end, ',')) {
			goto err;
		}
		if (-1 == tokenize(tokens[0], beg, end, ',')) {
			goto err;
		}
		if (-1 == parse(&iter, &beg, &end, ',')) {
			goto err;
		}
		if (-1 == tokenize(tokens[1], beg, end, ',')) {
			goto err;
		}
		if (-1 == parse(&iter, &beg, &end, '\n')) {
			goto err;
		}
		if (-1 == tokenize(tokens[2], beg, end, '\n')) {
			goto err;
		}
		type = tokens[0];
		question = tokens[1];
		answer = tokens[2];
		fprintf(stdout, "prompt: %s\n", question);
		errno = 0;
		if (-1 == getline(&textptr, &m, stdin)) {
			fprintf(stderr, "main: %s\n", strerror(errno));
			goto err;
		} else {
			fprintf(stdout, "input: %s", textptr);
			if (MAX_TOKEN_SIZE < (1 + strlen(textptr))) {
				fprintf(stderr, "main: %s\n", "TokenSizeError");
				goto err;
			}
			char *iter = textptr;
			char *beg = NULL;
			char *end = NULL;
			if (-1 == trim(&iter, &beg, &end)) {
				fprintf(stderr, "main: %s\n", "UXInputError");
				goto err;
			}
			memset(word, 0, MAX_TOKEN_SIZE);
			ssize_t const bytes = (end - beg);
			if (0 >= bytes) {
				fprintf(stderr, "main: %s\n", "UXInputSizeError");
				goto err;
			}
			memcpy(word, beg, bytes);
			fprintf(stdout, "word: %s\n", word);
		}
		if (!strcmp("math", type)) {
			long const res = atol(answer);
			fprintf(stdout, "result: %ld\n", res);
		} else if (!strcmp("text", type)) {
			fprintf(stdout, "result: %s\n", answer);
		} else {
			fprintf(stderr, "main: %s\n", "ProblemTypeError");
			goto err;
		}
	} while (sw);
	if (lineptr) {
		free(lineptr);
		lineptr = NULL;
		n = 0;
	}
	if (textptr) {
		free(textptr);
		textptr = NULL;
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
