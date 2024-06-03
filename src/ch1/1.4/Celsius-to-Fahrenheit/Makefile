#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/ch1/1.17/Celsius-to-Fahrenheit/Makefile
# author: @misael-diaz
#
# Synopsis:
# Defines the Makefile for building the program with GNU make.
#
# Copyright (c) 2024 Misael Díaz-Maldonado
# This file is released under the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#

include make-inc

all: $(C2F_BIN)

$(C2F_BIN): $(C2F_O)
	$(CC) $(CCOPT) $(C2F_O) -o $(C2F_BIN)

$(C2F_O): $(C2F_C)
	$(CC) $(CCOPT) -c $(C2F_C) -o $(C2F_O)

clean:
	/bin/rm -f *.o *.bin
