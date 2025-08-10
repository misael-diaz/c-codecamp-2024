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

ssize_t getTokens(
	char tokens[][MAX_TOKEN_SIZE],
	char const *lineptr
)
{
	char const *iter = lineptr;
	char const *beg = NULL;
	char const *end = NULL;
	if (-1 == parse(&iter, &beg, &end, ',')) {
		return -1;
	}
	if (-1 == tokenize(tokens[0], beg, end, ',')) {
		return -1;
	}
	if (-1 == parse(&iter, &beg, &end, ',')) {
		return -1;
	}
	if (-1 == tokenize(tokens[1], beg, end, ',')) {
		return -1;
	}
	if (-1 == parse(&iter, &beg, &end, '\n')) {
		return -1;
	}
	if (-1 == tokenize(tokens[2], beg, end, '\n')) {
		return -1;
	}
	return 0;
}

ssize_t prompt(
		char * const * const word,
		char ** const textptr,
		size_t * const textcap
)
{
	errno = 0;
	if (-1 == getline(textptr, textcap, stdin)) {
		fprintf(stderr, "prompt: %s\n", strerror(errno));
		return -1;
	}
	if (MAX_TOKEN_SIZE < (1 + strlen(*textptr))) {
		fprintf(stderr, "prompt: %s\n", "TokenSizeError");
		return -1;
	}
	char *iter = *textptr;
	char *beg = NULL;
	char *end = NULL;
	if (-1 == trim(&iter, &beg, &end)) {
		fprintf(stderr, "prompt: %s\n", "UXInputError");
		return -1;
	} else if (!beg || !end) {
		fprintf(stderr, "prompt: %s\n", "UXNullError");
		return -1;
	}
	memset(*word, 0, MAX_TOKEN_SIZE);
	ssize_t const bytes = (end - beg);
	if (0 >= bytes) {
		fprintf(stderr, "prompt: %s\n", "UXInputSizeError");
		return -1;
	}
	memcpy(*word, beg, bytes);
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
	int ncorrect = 0;
	int nproblems = 0;
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
		if (-1 == getTokens(tokens, lineptr)) {
			goto err;
		}
		type = tokens[0];
		question = tokens[1];
		answer = tokens[2];
		fprintf(stdout, "prompt: %s\n", question);
		char *w = &word[0];
		if (-1 == prompt(&w, &textptr, &m)) {
			goto err;
		}
		if (!strcmp("math", type)) {
			long const res = atol(answer);
			long const ans = atol(word);
			if (ans == res) {
				++ncorrect;
			}
		} else if (!strcmp("text", type)) {
			if (!strcasecmp(word, answer)) {
				++ncorrect;
			}
		} else {
			fprintf(stderr, "main: %s\n", "ProblemTypeError");
			goto err;
		}
		++nproblems;
	} while (sw);
	fprintf(stdout, "number of problems: %d\n", nproblems);
	fprintf(stdout, "number of correct problems: %d\n", ncorrect);
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
