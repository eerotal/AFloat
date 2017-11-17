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

int afloat_setd_str(char *digits, AFLOAT *ptr) {
	/*
	*  Copy the characters from the 'digits' string
	*  into the AFLOAT 'ptr'.
	*/
	char *tmp_str = strdup(digits);
	if (!tmp_str) {
		return AERROR_MEMORY;
	}
	for (size_t i = 0; i < strlen(tmp_str); i++) {
		/*
		* A simple way of converting numerical characters
		* to integer values from 0-9.
		*/
		tmp_str[i] -= 0x30;
	}
	afloat_setd(tmp_str, strlen(tmp_str), ptr);
	free(tmp_str);
	return AERROR_OK;
}

int afloat_setd(char *digits, size_t len, AFLOAT *ptr) {
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
	alog_printvv("AFLOAT reg'd (0x%x)", (unsigned int) ptr);

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

	alog_printvv("allocated (0x%x)", (unsigned int) ptr);
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
			aerror_printerr("AFLOAT unreg failed!");
		}
		free(ptr);
	}
}

void afloat_free_all(void) {
	/*
	*  Free all registered AFLOATs.
	*/
	alog_printv("AFLOAT free all!");
	while (afloats->len) {
		alog_printvv("AFLOAT free: 0x%x", afloats->elems[0].i);
		afloat_free((AFLOAT*) afloats->elems[0].p);
	}
}

