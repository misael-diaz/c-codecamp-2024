#!/usr/bin/make
#
# C-CodeCamp-2024					June 02, 2024
#
# source: src/Makefile
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

all: chapter1 go boot-dev

chapter1:
	@$(MAKE) -C ch1
go:
	@$(MAKE) -C golang
boot-dev:
	@$(MAKE) -C boot.dev
clean:
	@$(MAKE) -C ch1 clean
	@$(MAKE) -C golang clean
	@$(MAKE) -C boot.dev clean
