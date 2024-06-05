#include <stdio.h>

void swap(char *str);

int main ()
{
	char str_empty[] = "";
	swap(str_empty);
	char str_odd[] = "cat";
	swap(str_odd);
	char str_even[] = "abcdefghijklmnopqrstuvwxyz";
	swap(str_even);
	return 0;
}

static int length (const char *str)
{
	int count = 0;
	const char *it = str;
	while (*it) {
		++count;
		++it;
	}
	return count;
}

void swap (char *str)
{
	int const len = length(str);
	printf("len: %d\n", len);
	int num_swaps = (len / 2);
	int b = 0;
	int e = len;
	printf("original: %s\n", str);
	for (int i = 0; i != num_swaps; ++i) {
		char c = str[b];
		str[b] = str[e - 1];
		str[e - 1] = c;
		++b;
		--e;
	}
	printf("swapped:  %s\n", str);
}

/*

C-CodeCamp-2024                                       June 02, 2024

source: src/ch1/1.17/swap-string/swap-string.c
author: @misael-diaz

Synopsis:
Defines the include file for building the program with GNU make.

Copyright (c) 2024 Misael Díaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/
