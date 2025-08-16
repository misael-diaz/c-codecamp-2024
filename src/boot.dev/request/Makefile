#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/golang/tcplistener/Makefile
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

all: $(REQUEST_BIN)

$(REQUEST_BIN): $(REQUEST_O)
	$(CC) $(CCOPT) $(REQUEST_O) -o $(REQUEST_BIN)

$(REQUEST_O): $(HEADERS) $(REQUEST_C)
	$(CC) $(INC) $(CCOPT) -c $(REQUEST_C) -o $(REQUEST_O)

clean:
	/bin/rm -f *.o *.bin
