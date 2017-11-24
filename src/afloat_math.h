#ifndef INCLUDED_AFLOAT_MATH_
#define INCLUDED_AFLOAT_MATH_

#include "afloat.h"

void afloat_abs(AFLOAT *a);
int afloat_iszero(const AFLOAT *a);
int afloat_cmp(const AFLOAT *a, const AFLOAT *b);
int afloat_abs_cmp(const AFLOAT *a, const AFLOAT *b);
int afloat_sgn(const AFLOAT *ptr);
AFLOAT *afloat_sum(const AFLOAT *a, const AFLOAT *b);

#endif
