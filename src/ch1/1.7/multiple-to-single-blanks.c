#include <stdio.h>

int main ()
{
	char txt[] = "    hello    world    \n";
	int first_blank = 1;
	char *it = txt;
	while (*it) {
		if (*it == ' ') {
			if (first_blank) {
				printf("%c", *it);
				first_blank = 0;
			}
		} else {
			printf("%c", *it);
			first_blank = 1;
		}
		++it;
	}
	return 0;
}

/*

C-CodeCamp-2024                                       June 02, 2024

source: src/ch1/1.7/multiple-to-single-blanks.c
author: @misael-diaz

Synopsis:
Defines the include file for building the program with GNU make.

Copyright (c) 2024 Misael Díaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/
