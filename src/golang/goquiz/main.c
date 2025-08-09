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
	int sw = 0;
	do {
	} while (sw);
	return 0;
}
