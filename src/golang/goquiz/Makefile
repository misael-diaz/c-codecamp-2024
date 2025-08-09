#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/golang/goquiz/Makefile
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

all: $(GOQUIZ_BIN)

$(GOQUIZ_BIN): $(GOQUIZ_O)
	$(CC) $(CCOPT) $(GOQUIZ_O) -o $(GOQUIZ_BIN)

$(GOQUIZ_O): $(HEADERS) $(GOQUIZ_C)
	$(CC) $(INC) $(CCOPT) -c $(GOQUIZ_C) -o $(GOQUIZ_O)

clean:
	/bin/rm -f *.o *.bin
