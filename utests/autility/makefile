CC=gcc

CFLAGS=-std=gnu11 -pedantic-errors -pedantic -Wall -O -fPIC

DCFLAGS=
DLFLAGS=

NAME=autility

ifndef IDIR
$(error AFloat include directory not defined. Exiting)
endif

ifndef LDIR
$(error AFloat library directory not defined. Exiting)
endif

ifndef LNAME
$(error AFloat shared library file not defined. Exiting)
endif

ifndef VERBOSE
.SILENT:
endif

ifdef DEBUG
DCFLAGS=-g
DLFLAGS=-fsanitize=address -static-libasan
endif

.PHONY: clean

compile: $(wildcard src/*.c) $(wildcard src/*.h)
	$(CC) $(CFLAGS) $(DCFLAGS) $(DLFLAGS) $(wildcard src/*.c) \
		-o $(NAME).out -I$(IDIR) -L$(LDIR) -l$(LNAME)
	echo "Done. Run 'export LD_LIBRARY_PATH=$(LDIR)' before executing" \
		"the binary."

clean:
	rm -f $(NAME).out
