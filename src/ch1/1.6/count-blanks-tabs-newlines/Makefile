#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/ch1/1.6/count-blanks-tabs-newlines/Makefile
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

all: $(CBTN_BIN)

$(CBTN_BIN): $(CBTN_O)
	$(CC) $(CCOPT) $(CBTN_O) -o $(CBTN_BIN)

$(CBTN_O): $(CBTN_C)
	$(CC) $(CCOPT) -c $(CBTN_C) -o $(CBTN_O)

clean:
	/bin/rm -f *.o *.bin
