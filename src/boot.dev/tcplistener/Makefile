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

all: $(TCP_LISTENER_BIN)

$(TCP_LISTENER_BIN): $(TCP_LISTENER_O)
	$(CC) $(CCOPT) $(TCP_LISTENER_O) -o $(TCP_LISTENER_BIN)

$(TCP_LISTENER_O): $(HEADERS) $(TCP_LISTENER_C)
	$(CC) $(INC) $(CCOPT) -c $(TCP_LISTENER_C) -o $(TCP_LISTENER_O)

clean:
	/bin/rm -f *.o *.bin
