#include <stdio.h>

#include "afloat_discrete.h"
#include "afloat_math.h"
#include "aerror.h"

AFLOAT *afloat_sum(AFLOAT *a, const AFLOAT *b) {
	int err = AERROR_OK;
	int s = 0;
	int carry = 0;

	AITEM d;
	AFLOAT *sum = afloat_define();
	AFLOAT *a_tmp = afloat_dup(a);
	AFLOAT *b_tmp = afloat_dup(b);

	if (!a_tmp || !b_tmp) {
		return NULL;
	}

	AERROR_FLGP(afloat_pad(a_tmp, b_tmp), err);
	AERROR_FLGP(afloat_reverse(a_tmp), err);
	AERROR_FLGP(afloat_reverse(b_tmp), err);

	if (!AERROR_CHKFLG(err)) {
		aerror_printerr("Math operation(s) failed!");
		afloat_free(a_tmp);
		afloat_free(b_tmp);
		return NULL;
	}

	for (size_t i = 0; i < a_tmp->d->len; i++) {
		s = a_tmp->d->elems[i].c + b_tmp->d->elems[i].c;
		d.c = (s % 10) + carry;
		if (!array_put(sum->d, &d)) {
			afloat_free(a_tmp);
			afloat_free(b_tmp);
			afloat_free(sum);
			return NULL;

		}
		carry = (s - s % 10)/10;
	}
	afloat_free(a_tmp);
	afloat_free(b_tmp);

	if (carry) {
		d.c = (s % 10) + carry;
		if (!array_put(sum->d, &d)) {
			afloat_free(sum);
			return NULL;
		}
	}

	if (!AERROR_CHKP(afloat_reverse(sum))) {
		afloat_free(sum);
		return NULL;
	}

	return sum;
}
