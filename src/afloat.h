#ifndef INCLUDED_AFLOAT_
#define INCLUDED_AFLOAT_

#include <stdlib.h>
#include <stdint.h>

#include "array.h"

#define AFLOAT_SGN_NEG -1
#define AFLOAT_SGN_POS 1

typedef struct {
	int sgn;
	ARR *d;
} AFLOAT;

void afloat_init(void);
void afloat_destroy(void);
void afloat_print(const AFLOAT *ptr);
ARR *afloat_getd(const AFLOAT *ptr);
size_t afloat_getd_len(const AFLOAT *ptr);
int afloat_setd_str(AFLOAT *ptr, char *digits);
int afloat_setd(AFLOAT *ptr, char *digits, size_t len, int sgn);
void afloat_cleard(AFLOAT *ptr);
AFLOAT *afloat_define(void);
void afloat_dump_all(void);
void afloat_free(AFLOAT *ptr);
void afloat_free_all(void);

#endif
