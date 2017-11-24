#include <stdio.h>
#include <stdlib.h>

#include "aerror.h"

char AERROR_STRS[][50] = {
	"No errors",
	"Invalid pointer in function arguments",
	"Invalid function argument(s)",
	"Memory allocation error",
	"Internal error",
	"Unknown error"
};

char *aerror_strerr(enum AERROR err) {
	if (abs(err) >= abs(AERROR_UNKNOWN)) {
		return AERROR_STRS[abs(AERROR_UNKNOWN)];
	}
	return AERROR_STRS[abs(err)];
}

enum AERROR _aerror_printerr_num(enum AERROR err, int force,
				int ln, char *f) {
	if (err != AERROR_OK || force) {
		printf("AERROR: (%i) %s:%i: %s.\n", err, f, ln,
			aerror_strerr(err));
	}
	return err;
}

void _aerror_printerr_custom(char *s, int ln, char *f) {
	printf("AERROR: %i:%s: %s\n", ln, f, s);
}
