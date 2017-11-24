CC=gcc

CFLAGS=-c -std=gnu11 -pedantic-errors -pedantic -Wall -O -fPIC
LFLAGS=-shared

DCFLAGS=
DLFLAGS=

NAME=afloat

UTESTS=$(wildcard utests/*/)

ifndef VERBOSE
.SILENT:
endif

ifdef DEBUG
DCFLAGS=-g
DLFLAGS=-fsanitize=address -static-libasan
endif

.PHONY: clean utests $(UTESTS) LOC

compile: $(wildcard src/*.c) $(wildcard src/*.h)
	$(CC) $(CFLAGS) $(DCFLAGS) $(wildcard src/*.c)
	$(CC) $(LFLAGS) $(DLFLAGS) -o lib$(NAME).so *.o
	rm -f *.o
clean:
	rm -f $(NAME).out

utests: $(UTESTS)
$(UTESTS):
	echo ">>> Compiling '$@'..."
	make -C $@ LDIR=$(shell pwd) IDIR=$(shell pwd)/src \
		LNAME=$(NAME) DEBUG=$(DEBUG) VERBOSE=$(VERBOSE)
	echo ">>> Done."

LOC:
	wc -l src/*.c src/*.h
