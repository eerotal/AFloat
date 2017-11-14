#include <stdio.h>

#include "afloat_discrete.h"
#include "afloat_math.h"

int _afloat_sum__add_digit(unsigned int d, AFLOAT *sum) {
	AITEM digit;
	AITEM *tmp_digits = NULL;

	digit.c = d;
	tmp_digits = array_put(sum->d, &digit, sum->d_len);
	if (!tmp_digits) {
		return -1;
	}
	sum->d = tmp_digits;
	sum->d_len++;
	return 1;
}

AFLOAT *afloat_sum(AFLOAT *a, const AFLOAT *b) {
	int s = 0;
	int carry = 0;
	AFLOAT *sum = afloat_define();
	AFLOAT *tmp_sum = NULL;
	AFLOAT *a_rev = NULL;
	AFLOAT *b_rev = NULL;
	AFLOAT *a_pad = afloat_dup(a);
	AFLOAT *b_pad = afloat_dup(b);

	if (!a_pad || !b_pad) {
		return NULL;
	}

	if (!afloat_pad(a_pad, b_pad)) {
		afloat_free(a_pad);
		afloat_free(b_pad);
		return NULL;
	}

	a_rev = afloat_reverse(a_pad);
	b_rev = afloat_reverse(b_pad);

	afloat_free(a_pad);
	afloat_free(b_pad);

	if (!a_rev || !b_rev) {
		return NULL;
	}

	for (size_t i = 0; i < a_rev->d_len; i++) {
		s = a_rev->d[i].c + b_rev->d[i].c;
		if (!_afloat_sum__add_digit((s % 10) + carry, sum)) {
			afloat_free(a_rev);
			afloat_free(b_rev);
			afloat_free(sum);
			return NULL;

		}
		carry = (s - s % 10)/10;
	}
	if (carry) {
		_afloat_sum__add_digit(carry, sum);
		afloat_free(a_rev);
		afloat_free(b_rev);
		afloat_free(sum);
			return NULL;
	}

	tmp_sum = afloat_reverse(sum);

	afloat_free(a_rev);
	afloat_free(b_rev);
	afloat_free(sum);
	return tmp_sum;
}
