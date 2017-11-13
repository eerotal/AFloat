#include "afloat_discrete.h"
#include "afloat_math.h"

AFLOAT *add(AFLOAT *a, const AFLOAT *b) {
	AFLOAT *a_rev = afloat_reverse(a);
	AFLOAT *b_rev = afloat_reverse(b);
	if (!a_rev || !b_rev) {
		return NULL;
	}
}
