#include <stdio.h>

#define BLANK_CHAR ' '
#define TAB_CHAR '\t'
#define NEWLINE_CHAR '\n'

#define NUM_MATCH_CHARS 3

enum matchchar {
	BLANK,
	TAB,
	NEWLINE
};

static char names[NUM_MATCH_CHARS][8] = {"blanks", "tabs", "newlines"};
static size_t count[NUM_MATCH_CHARS];

size_t map(char c);

int main ()
{
	char txt[] = "abcd    \t\t\t\t\n\n\n\n";
	char *iter = txt;
	while (*iter) {
		if (map(*iter) != NUM_MATCH_CHARS) {
			++count[map(*iter)];
		}
		++iter;
	}

	for (size_t i = 0; i != NUM_MATCH_CHARS; ++i) {
		printf("%s: %zu\n", names[i], count[i]);
	}

	return 0;
}

size_t map (char c)
{
	switch (c) {
		case BLANK_CHAR:
			return BLANK;
		case TAB_CHAR:
			return TAB;
		case NEWLINE_CHAR:
			return NEWLINE;
		default:
			return NUM_MATCH_CHARS;
	}
}

/*

C-CodeCamp-2024                                       June 02, 2024

source: src/ch1/1.6/count-blanks-tabs-newlines/count-blanks-tabs-newlines.c
author: @misael-diaz

Synopsis:
Defines the include file for building the program with GNU make.

Copyright (c) 2024 Misael Díaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/
