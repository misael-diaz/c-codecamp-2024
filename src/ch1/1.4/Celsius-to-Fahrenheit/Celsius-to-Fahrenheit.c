#include <stdio.h>

#define CELSIUS_LOWER_BOUND   0.0
#define CELSIUS_UPPER_BOUND 100.0
#define CELSIUS_STEP_SIZE    20.0

int main ()
{
	double clb = CELSIUS_LOWER_BOUND;
	double cub = CELSIUS_UPPER_BOUND;
	double stp = CELSIUS_STEP_SIZE;
	printf("     Celsius \t  Fahrenheit\n");
	printf("============================\n");
	for (double C = clb; C <= cub; C += stp) {
		double const Fahr = (1.8 * C + 32.0);
		printf("%12.1lf\t%12.1lf\n", C, Fahr);
	}
	return 0;
}

/*

C-CodeCamp-2024                                       June 02, 2024

source: src/ch1/1.4/Celsius-to-Farentheit.c
author: @misael-diaz

Synopsis:
Defines the include file for building the program with GNU make.

Copyright (c) 2024 Misael Díaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

*/
