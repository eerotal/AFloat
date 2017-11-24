CC=gcc
CFLAGS=-std=gnu11 -pedantic-errors -pedantic -Wall -O -fsanitize=address -static-libasan -g
NAME=afloat

ifndef VERBOSE
.SILENT:
endif

.PHONY: clean LOC

compile: $(wildcard src/*.c)
	$(CC) $(CFLAGS) $(wildcard src/*.c) -o $(NAME).out

clean:
	rm -f $(NAME).out

LOC:
	wc -l src/*.c src/*.h
