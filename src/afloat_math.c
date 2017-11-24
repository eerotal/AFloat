#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afloat_discrete.h"
#include "afloat_math.h"
#include "aerror.h"

#define AMATH_ALGO_RUNNING   0
#define AMATH_ALGO_FINISHED  1

#define AMATH_ALGO_IGNORE    0
#define AMATH_ALGO_USE       1

typedef struct {
	int error;
	int finished;
	int ignore;
} RDATA;

typedef int (*afloat_algo)(const AFLOAT *a, const AFLOAT *b,
					void **state, RDATA *ret);

static AFLOAT *_afloat_op_nochecks(const AFLOAT *a, const AFLOAT *b,
					afloat_algo op);

// Algorithm implementations
static int _afloat_algo_lsum_raw(const AFLOAT *a, const AFLOAT *b,
				void **state, RDATA *ret);
static int _afloat_algo_lsub_raw(const AFLOAT *a, const AFLOAT *b,
					void **state, RDATA *ret);

void afloat_abs(AFLOAT *a) {
	/*
	*  Make 'a' the absolute value of 'a'.
	*  a >= 0 => a = a;
	*  a <  0 => a = -a;
	*/
	a->sgn = AFLOAT_SGN_POS;
}

int afloat_iszero(const AFLOAT *a) {
	/*
	*  Return 1 if 'a' == 0 and 0 otherwise.
	*/
	for (size_t i = 0; i < a->d->len; i++) {
		if (array_get(a->d, i)->c != 0) {
			return 0;
		}
	}
	return 1;
}

int afloat_cmp(const AFLOAT *a, const AFLOAT *b) {
	/*
	*  Compare the values of 'a' and 'b'. Return values are
	*  as follows:
	*    ERROR  -> -2
	*    a < b  -> -1
	*    a = b  ->  0
	*    a > b  ->  1
	*/
	int ret = 0;
	char a_d = 0;
	char b_d = 0;
	AFLOAT *a_tmp = afloat_dup(a);
	AFLOAT *b_tmp = afloat_dup(b);

	if (!a_tmp || !b_tmp) {
		goto error;
	}

	if (!AERROR_CHKP(afloat_pad(a_tmp, b_tmp))) {
		goto error;
	}

	for (size_t i = 0; i < a_tmp->d->len; i++) {
		a_d = afloat_sgn(a)*array_get(a_tmp->d, i)->c;
		b_d = afloat_sgn(b)*array_get(b_tmp->d, i)->c;
		if (a_d < b_d) {
			ret = -1;
			break;
		} else if (a_d > b_d) {
			ret = 1;
			break;
		}
	}

	afloat_free(a_tmp);
	afloat_free(b_tmp);
	return ret;

	error:
		if (a_tmp) { afloat_free(a_tmp); }
		if (b_tmp) { afloat_free(b_tmp); }
		return -2;
}

int afloat_abs_cmp(const AFLOAT *a, const AFLOAT *b) {
	/*
	*  Compare the absolute values of 'a' and 'b'.
	*  See afloat_abs() for return values.
	*/
	int ret = 0;
	AFLOAT *a_tmp = afloat_dup(a);
	AFLOAT *b_tmp = afloat_dup(b);

	if (!a_tmp || !b_tmp) {
		goto error;
	}

	afloat_abs(a_tmp);
	afloat_abs(b_tmp);

	ret = afloat_cmp(a_tmp, b_tmp);

	afloat_free(a_tmp);
	afloat_free(b_tmp);
	return ret;

	error:
		if (a_tmp) { afloat_free(a_tmp); }
		if (b_tmp) { afloat_free(b_tmp); }
		return -2;
}

int afloat_sgn(const AFLOAT *ptr) {
	/*
	*  Return AFLOAT_SGN_NEG/POS based on the sign
	*  of 'ptr'.
	*/
	if (ptr->sgn == AFLOAT_SGN_NEG) {
		return -1;
	} else {
		return 1;
	}
}

AFLOAT *afloat_sum(const AFLOAT *a, const AFLOAT *b) {
	/*
	*  AFloat addition wrapper function. The algorithm
	*  worker functions _afloat_algo_(lsub/lsum)_raw do all the
	*  heavy lifting. This function works with signed
	*  numbers too as opposed to the raw algorithm functions.
	*  Returns a pointer to a new AFLOAT instance on success
	*  or a NULL pointer on failure.
	*/

	int cmp = 0;
	AFLOAT *tmp = NULL;

	if (afloat_sgn(a) == afloat_sgn(b)) {
		//  a+b =  (a+b) = sgn(a)*(a+b)
		// -a-b = -(a+b) = sgn(a)*(a+b)
		return _afloat_op_nochecks(a, b, _afloat_algo_lsum_raw);
	} else if (afloat_sgn(a) == -1 && afloat_sgn(b) == 1) {
		// -a+b = -(a-b)  | a >= b
		// -a+b = b-a     | a < b
		cmp = afloat_abs_cmp(a, b);
		if (cmp >= 0) {
			tmp = _afloat_op_nochecks(a, b,
					_afloat_algo_lsub_raw);
			tmp->sgn = AFLOAT_SGN_NEG;
			return tmp;
		} else if (cmp == -1) {
			return _afloat_op_nochecks(b, a,
					_afloat_algo_lsub_raw);
		} else {
			aerror_printerr("AFloat comparison error!");
			return NULL;
		}
	} else if (afloat_sgn(a) == 1 && afloat_sgn(b) == -1) {
		// a+(-b) = a-b     | a >= b
		// a+(-b) = -(b-a)  | a < b
		return afloat_sum(b, a);
	}
	aerror_printerr("Invalid AFloat signs for addition!");
	return NULL;
}

static int _afloat_algo_lsub_raw(const AFLOAT *a, const AFLOAT *b,
				void **state, RDATA *ret) {
	/*
	*  Subtraction algorithm worker function. The result is only
	*  defined when a > b > 0. Otherwise the result is undefined.
	*  This algorithm works on the "raw" afloat digit data, which
	*  means it doesn't take the sign of the numbers into account.
	*/

	int d_sub = 0;
	struct s_t {
		int carry;
		size_t i;
	} s = { 0 };

	if (*(struct s_t**) state) {
		memcpy(&s, *(struct s_t**) state, sizeof(struct s_t));
	} else {
		*state = calloc(1, sizeof(s));
		if (!(*(struct s_t**) state)) {
			ret->error = AERROR_MEMORY;
			ret->finished = AMATH_ALGO_FINISHED;
			return 0;
		}
	}
	memset(ret, 0, sizeof(*ret));

	// ##### Subtraction algorithm
	ret->finished = AMATH_ALGO_RUNNING;
	ret->ignore = AMATH_ALGO_USE;

	if (s.i == a->d->len) {
		ret->finished = AMATH_ALGO_FINISHED;
		d_sub = s.carry;
		if (!d_sub) {
			ret->ignore = AMATH_ALGO_IGNORE;
		}
	} else {
		d_sub = array_get(a->d, s.i)->c -
			array_get(b->d, s.i)->c -
			s.carry;
		if (d_sub < 0) {
			d_sub += 10;
			s.carry = 1;
		} else {
			s.carry = 0;
		}
	}
	s.i++;
	// #####

	memcpy(*(struct s_t**) state, &s, sizeof(struct s_t));

	return d_sub;
}

static int _afloat_algo_lsum_raw(const AFLOAT *a, const AFLOAT *b,
				void **state, RDATA *ret) {
	/*
	*  Addition algorithm worker function. The result is only defined
	*  when a > 0 and b > 0. Otherwise the result is undefined.
	*  This algorithm works on the "raw" afloat digit data, which
	*  means it doesn't take the sign of the numbers into account.
	*/

	int d_sum = 0;
	struct s_t {
		int carry;
		size_t i;
	} s = { 0 };

	if (*(struct s_t**) state) {
		memcpy(&s, *(struct s_t**) state, sizeof(struct s_t));
	} else {
		*state = calloc(1, sizeof(s));
		if (!(*(struct s_t**) state)) {
			ret->error = AERROR_MEMORY;
			ret->finished = AMATH_ALGO_FINISHED;
			return 0;
		}
	}
	memset(ret, 0, sizeof(*ret));

	// ##### Addition algorithm
	ret->finished = AMATH_ALGO_RUNNING;
	ret->ignore = AMATH_ALGO_USE;

	if (s.i == a->d->len) {
		ret->finished = AMATH_ALGO_FINISHED;
		d_sum = s.carry;
		if (!d_sum) {
			ret->ignore = AMATH_ALGO_IGNORE;
		}
	} else {
		d_sum = array_get(a->d, s.i)->c +
			array_get(b->d, s.i)->c +
			s.carry;
		s.carry = (d_sum - d_sum % 10)/10;
	}
	s.i++;
	// #####

	memcpy(*(struct s_t**) state, &s, sizeof(struct s_t));

	return d_sum % 10;
}

static AFLOAT *_afloat_op_nochecks(const AFLOAT *a,
				const AFLOAT *b,
				afloat_algo op) {
	/*
	*  Run an algorithm worker function on two AFLOAT instances.
	*  Returns a pointer to a new AFLOAT instance on success or
	*  a NULL pointer on failure.
	*/

	void *state = NULL;
	RDATA ret = { 0 };
	int err = AERROR_OK;

	AITEM d;
	AFLOAT *res = afloat_define();
	AFLOAT *a_tmp = afloat_dup(a);
	AFLOAT *b_tmp = afloat_dup(b);

	if (!a_tmp || !b_tmp) {
		goto error;
	}

	AERROR_FLGP(afloat_pad(a_tmp, b_tmp), err);
	AERROR_FLGP(afloat_reverse(a_tmp), err);
	AERROR_FLGP(afloat_reverse(b_tmp), err);

	if (!AERROR_CHKFLG(err)) {
		aerror_printerr("Math operation(s) failed!");
		goto error;
	}

	afloat_print(a_tmp);
	printf("\n");
	afloat_print(b_tmp);
	printf("\n");

	while (ret.finished == AMATH_ALGO_RUNNING &&
		AERROR_CHK(ret.error)) {
		d.c = (*op)(a_tmp, b_tmp, &state, &ret);
		if (ret.ignore == AMATH_ALGO_USE) {
			if (!array_put(res->d, &d)) {
				aerror_printerr("Array operation failed!");
				goto error;
			}
		}
	}

	if (!AERROR_CHKP(ret.error)) {
		aerror_printerr("Algorithm failed!");
		goto error;
	}

	if (!AERROR_CHKP(afloat_reverse(res))) {
		aerror_printerr("AFloat reverse failed!");
		goto error;
	}

	afloat_free(a_tmp);
	afloat_free(b_tmp);

	return res;

	error:
		if (a_tmp) { afloat_free(a_tmp); }
		if (b_tmp) { afloat_free(b_tmp); }
		if (res) { afloat_free(res); }
		return NULL;
}
