#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "afloat.h"

static unsigned int afloats_len = 0;
static AITEM *afloats = NULL;

void afloat_print(const AFLOAT *ptr) {
	for (size_t i = 0; i < ptr->d_len; i++) {
		printf("%i", ptr->d[i].c);
	}
}

AITEM *afloat_getd(const AFLOAT *ptr) {
	/*
	*  Get AFLOAT digit array.
	*/
	return ptr->d;
}

size_t afloat_getd_len(const AFLOAT *ptr) {
	/*
	*  Get AFLOAT digit count.
	*/
	return ptr->d_len;
}

int afloat_setd_str(char *digits, AFLOAT *ptr) {
	/*
	*  Copy the characters from the 'digits' string
	*  into the AFLOAT 'ptr'.
	*/
	char *tmp_str = strdup(digits);
	if (!tmp_str) {
		return -1;
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
	return 1;
}

int afloat_setd(char *digits, size_t len, AFLOAT *ptr) {
	/*
	*  Copy the digits from the 'digits' array into
	*  the AFLOAT 'ptr'.
	*/
	AITEM *arr = NULL;
	if (!digits) {
		afloat_cleard(ptr);
		return 1;
	}

	arr = array_set(ptr->d, digits, len, AITEM_CHAR);
	if (!arr) {
		return -1;
	}
	ptr->d = arr;
	ptr->d_len = len;

	return 1;
}

void afloat_cleard(AFLOAT *ptr) {
	/*
	*  Clear the existing digits from 'ptr'.
	*/
	if (ptr->d) {
		free(ptr->d);
		ptr->d = NULL;
		ptr->d_len = 0;
	}
}

static int _afloat_reg(AFLOAT *ptr) {
	/*
	*  Register an AFLOAT instance.
	*/
	AITEM *ret = NULL;
	AITEM tmp;

	tmp.p = (void*) ptr;

	ret = array_put(afloats, &tmp, afloats_len);
	if (!ret) {
		return -1;
	}
	afloats = ret;
	afloats_len++;
	printf("AFLOAT reg'd (0x%x)\n", (unsigned int) ptr);

	return 1;
}

static int _afloat_unreg(AFLOAT *ptr) {
	/*
	*  Unregister an AFLOAT instance.
	*/
	AITEM *ret = NULL;
	for (size_t i = 0; i < afloats_len; i++) {
		if ((AFLOAT*) afloats[i].p == ptr) {
			ret = array_pop(afloats, i, afloats_len);
			if (!ret) {
				return -1;
			}

			afloats = ret;
			afloats_len--;
			return 1;
		}
	}
	return -1;
}

AFLOAT *afloat_define(void) {
	/*
	*  Define an AFLOAT instance. Returns a pointer
	*  to an AFLOAT instance on success and NULL on failure.
	*/
	AFLOAT *ptr = calloc(1, sizeof(AFLOAT));
	printf("AFLOAT calloc'd (0x%x)\n", (unsigned int) ptr);
	if (!_afloat_reg(ptr)) {
		printf("Failed to register AFLOAT instance.\n");
		free(ptr);
		return NULL;
	}
	return ptr;
}

void afloat_dump_all(void) {
	/*
	*  Dump the pointer hex codes of all registered AFLOATs.
	*/
	printf("AFLOAT Dump:\n");
	for (size_t i = 0; i < afloats_len; i++) {
		printf("\t%i: 0x%x\n", i, afloats[i].i);
	}
}

void afloat_free(AFLOAT *ptr) {
	if (ptr) {
		if (ptr->d) {
			free(ptr->d);
			ptr->d = NULL;
		}
		if (!_afloat_unreg(ptr)) {
			printf("AFLOAT unreg failed!\n");
		}
		free(ptr);
	}
}

void afloat_free_all(void) {
	/*
	*  Free all registered AFLOATs.
	*/
	printf("AFLOAT free all!\n");
	while (afloats_len) {
		printf("AFLOAT free: 0x%x\n", afloats[0].i);
		afloat_free((AFLOAT*) afloats[0].p);
	}
}
