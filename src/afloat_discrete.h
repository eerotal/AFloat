#include "afloat.h"

#ifndef INCLUDED_AFLOAT_DISCRETE_
#define INCLUDED_AFLOAT_DISCRETE_

AFLOAT *afloat_dup(const AFLOAT *ptr);
AFLOAT *afloat_reverse(const AFLOAT *ptr);
int afloat_pad(AFLOAT *a, AFLOAT *b);

#endif
