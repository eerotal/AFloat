#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array.h"
#include "afloat_discrete.h"

int afloat_pad(AFLOAT *a, AFLOAT *b) {
	/*
	*  Pad the AFLOAT values 'a' and 'b' so that they
	*  are equal in length. The padding is done by adding
	*  leading zeroes to the AFLOAT digit arrays.
	*/

	AITEM filler;
	AITEM *tmp = NULL;

	filler.c = 0;

	if (!a->d || !b->d) {
		return -1;
	}

	if (a->d_len == b->d_len) {
		return 1;
	} else if (a->d_len > b->d_len) {
		tmp = array_pad(b->d, b->d_len, a->d_len, &filler, -1);
		if (tmp) {
			b->d = tmp;
			b->d_len = a->d_len;
			return 1;
		}
	} else {
		tmp = array_pad(a->d, a->d_len, b->d_len, &filler, -1);
		if (tmp) {
			a->d = tmp;
			a->d_len = b->d_len;
			return 1;
		}
	}
	return -1;
}

AFLOAT *afloat_dup(const AFLOAT *ptr) {
	/*
	*  Return a pointer to a copy of 'ptr' or
	*  NULL on failure.
	*/
	AFLOAT *tmp = NULL;
	tmp = afloat_define();
	tmp->sign = ptr->sign;

	if (ptr->d) {
		tmp->d = malloc(ptr->d_len*sizeof(*ptr->d));
		if (!tmp->d) {
			afloat_free(tmp);
			return NULL;
		}
		tmp->d_len = ptr->d_len;

		memcpy(tmp->d, ptr->d, ptr->d_len*sizeof(*ptr->d));
	} else {
		tmp->d = NULL;
		tmp->d_len = 0;
	}
	return tmp;

}

AFLOAT *afloat_reverse(const AFLOAT *ptr) {
	/*
	*  Return an AFLOAT instance with the digits of
	*  'ptr' reversed or NULL on failure.
	*/

	AFLOAT *rev = NULL;
	if (!ptr->d) {
		return NULL;
	}

	rev = afloat_dup(ptr);
	for (size_t i = 0; i < ptr->d_len; i++) {
		rev->d[i] = ptr->d[ptr->d_len - 1 - i];
	}

	return rev;
}
