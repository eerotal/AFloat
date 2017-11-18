#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array.h"
#include "afloat_discrete.h"
#include "aerror.h"

int afloat_pad(AFLOAT *a, AFLOAT *b) {
	/*
	*  Pad the AFLOAT values 'a' and 'b' so that they
	*  are equal in length. The padding is done by adding
	*  leading zeroes to the AFLOAT digit arrays.
	*/

	AITEM filler;
	filler.c = 0;

	if (!a || !b || !a->d->elems || !b->d->elems) {
		return AERROR_INVALID_ARGS;
	}

	if (a->d->len == b->d->len) {
		return AERROR_OK;
	} else if (a->d->len > b->d->len) {
		if (array_pad(b->d, a->d->len, &filler, -1)) {
			return AERROR_OK;
		}
	} else {
		if (array_pad(a->d, b->d->len, &filler, -1)) {
			return AERROR_OK;
		}
	}
	return AERROR_INTERNAL;
}

AFLOAT *afloat_dup(const AFLOAT *ptr) {
	/*
	*  Return a pointer to a copy of 'ptr' or
	*  NULL on failure.
	*/
	AFLOAT *tmp = NULL;
	tmp = afloat_define();
	tmp->sgn = ptr->sgn;

	if (ptr->d) {
		tmp->d->elems = malloc(ptr->d->len*sizeof(*ptr->d->elems));
		if (!tmp->d->elems) {
			afloat_free(tmp);
			return NULL;
		}
		tmp->d->len = ptr->d->len;

		memcpy(tmp->d->elems, ptr->d->elems,
			ptr->d->len*sizeof(*ptr->d->elems));
	} else {
		tmp->d->elems = NULL;
		tmp->d->len = 0;
	}
	return tmp;

}

int afloat_reverse(AFLOAT *ptr) {
	AITEM *rev = NULL;
	if (!ptr || !ptr->d->elems || !ptr->d->len) {
		return AERROR_INVALID_ARGS;
	}

	rev = calloc(ptr->d->len, sizeof(*ptr->d->elems));
	if (!rev) {
		return AERROR_MEMORY;
	}

	size_t i = ptr->d->len;
	while(i--) {
		rev[ptr->d->len - 1 - i].c = ptr->d->elems[i].c;
	}

	free(ptr->d->elems);
	ptr->d->elems = rev;
	return AERROR_OK;
}
