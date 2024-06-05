#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/ch1/1.17/Makefile
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

all: $(SWAP_STRING_BIN)

$(SWAP_STRING_BIN): $(SWAP_STRING_O)
	$(CC) $(CCOPT) $(SWAP_STRING_O) -o $(SWAP_STRING_BIN)

$(SWAP_STRING_O): $(HEADERS) $(SWAP_STRING_C)
	$(CC) $(INC) $(CCOPT) -c $(SWAP_STRING_C) -o $(SWAP_STRING_O)

clean:
	/bin/rm -f *.o *.bin
