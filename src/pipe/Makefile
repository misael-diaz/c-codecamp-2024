#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/ch1/1.11/Makefile
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

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_O)
	$(CC) $(CCOPT) $(TEST_O) -o $(TEST_BIN)

$(TEST_O): $(HEADERS) $(TEST_C)
	$(CC) $(INC) $(CCOPT) -c $(TEST_C) -o $(TEST_O)

clean:
	/bin/rm -f *.o *.bin
