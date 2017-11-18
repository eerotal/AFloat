#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "afloat.h"
#include "aerror.h"
#include "alog.h"

static ARR *afloats = NULL;

void afloat_init(void) {
	afloats = calloc(1, sizeof(ARR));
}

void afloat_destroy(void) {
	if (afloats) {
		afloat_free_all();
		free(afloats);
		afloats = NULL;
	}
}

void afloat_print(const AFLOAT *ptr) {
	if (ptr) {
		if (ptr->sgn == AFLOAT_SGN_NEG) {
			printf("-");
		} else {
			printf("+");
		}
		for (size_t i = 0; i < ptr->d->len; i++) {
			printf("%i", ptr->d->elems[i].c);
		}
	}
}

ARR *afloat_getd(const AFLOAT *ptr) {
	/*
	*  Get AFLOAT digit array.
	*/
	return ptr->d;
}

size_t afloat_getd_len(const AFLOAT *ptr) {
	/*
	*  Get AFLOAT digit count.
	*/
	return ptr->d->len;
}

int afloat_setd_str(AFLOAT *ptr, char *digits) {
	/*
	*  Copy the characters from the 'digits' string
	*  into the AFLOAT 'ptr'. The first character in
	*  'digits' can be '-' or '+' signifying the sign
	*  of the number. If no sign is specified, a
	*  positive number is created.
	*/
	size_t str_start = 0;
	char *tmp_str = strdup(digits);
	int sgn = 0;

	if (!tmp_str) {
		return AERROR_MEMORY;
	}

	if (digits[0] == '-') {
		if (strlen(digits) == 1) {
			return AERROR_INVALID_ARGS;
		}
		sgn = AFLOAT_SGN_NEG;
		str_start = 1;
	} else if (digits[0] == '+') {
		if (strlen(digits) == 1) {
			return AERROR_INVALID_ARGS;
		}
		sgn = AFLOAT_SGN_POS;
		str_start = 1;
	} else {
		sgn = AFLOAT_SGN_POS;
	}

	for (size_t i = str_start; i < strlen(tmp_str); i++) {
		/*
		* A simple way of converting numerical characters
		* to integer values from 0-9.
		*/
		if (tmp_str[i] < 0x30 || tmp_str[i] > 0x39) {
			free(tmp_str);
			return AERROR_INVALID_ARGS;
		}
		tmp_str[i] -= 0x30;
	}
	if (!AERROR_CHKP(afloat_setd(ptr, tmp_str + str_start,
		strlen(tmp_str) - str_start, sgn))) {

		free(tmp_str);
		return AERROR_INTERNAL;
	}
	free(tmp_str);
	return AERROR_OK;
}

int afloat_setd(AFLOAT *ptr, char *digits, size_t len, int sgn) {
	/*
	*  Copy the digits from the 'digits' array into
	*  the AFLOAT 'ptr'.
	*/
	if (!digits) {
		afloat_cleard(ptr);
		return AERROR_OK;
	}

	if (!array_set(ptr->d, digits, len, AITEM_CHAR)) {
		return AERROR_INTERNAL;
	}
	ptr->sgn = sgn;

	return AERROR_OK;
}

void afloat_cleard(AFLOAT *ptr) {
	/*
	*  Clear the existing digits from 'ptr'.
	*/
	if (ptr->d) {
		free(ptr->d);
		ptr->d = NULL;
		ptr->d->len = 0;
		ptr->sgn = 0;
	}
}

static int _afloat_reg(AFLOAT *ptr) {
	/*
	*  Register an AFLOAT instance.
	*/
	AITEM tmp;

	tmp.p = (void*) ptr;
	if (!array_put(afloats, &tmp)) {
		return AERROR_INTERNAL;
	}
	alog_printvv("AFloat reg'd @ (0x%x)", (unsigned int) ptr);

	return AERROR_OK;
}

static int _afloat_unreg(AFLOAT *ptr) {
	/*
	*  Unregister an AFLOAT instance.
	*/
	for (size_t i = 0; i < afloats->len; i++) {
		if ((AFLOAT*) afloats->elems[i].p == ptr) {
			array_pop(afloats, i);
			return AERROR_OK;
		}
	}
	return AERROR_INVALID_ARGS;
}

AFLOAT *afloat_define(void) {
	/*
	*  Define an AFLOAT instance. Returns a pointer
	*  to an AFLOAT instance on success and NULL on failure.
	*/
	AFLOAT *ptr = calloc(1, sizeof(AFLOAT));
	ptr->d = calloc(1, sizeof(ARR));

	alog_printvv("AFloat allocated @ (0x%x)", (unsigned int) ptr);
	if (!AERROR_CHKP(_afloat_reg(ptr))) {
		aerror_printerr("Failed to register AFLOAT instance.");
		free(ptr);
		return NULL;
	}
	return ptr;
}

void afloat_dump_all(void) {
	/*
	*  Dump the pointer hex codes of all registered AFLOATs.
	*/
	alog_printv("AFLOAT Dump:");
	for (size_t i = 0; i < afloats->len; i++) {
		alog_printv("\t%i: 0x%x", i, afloats->elems[i].i);

	}
}

void afloat_free(AFLOAT *ptr) {
	if (ptr) {
		if (ptr->d) {
			free(ptr->d);
			ptr->d = NULL;
		}
		if (!AERROR_CHKP(_afloat_unreg(ptr))) {
			aerror_printerr("AFloat unreg failed!");
		}
		free(ptr);
	}
}

void afloat_free_all(void) {
	/*
	*  Free all registered AFLOATs.
	*/
	alog_printv("Free all AFloats!");
	while (afloats->len) {
		alog_printvv("AFloat free: 0x%x", afloats->elems[0].i);
		afloat_free((AFLOAT*) afloats->elems[0].p);
	}
}

