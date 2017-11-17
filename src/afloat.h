#ifndef INCLUDED_AFLOAT_
#define INCLUDED_AFLOAT_

#include <stdlib.h>
#include <stdint.h>

#include "array.h"

#define AFLOAT_ERR_ACC(f, v_err)  v_err -= (!f ? 1 : 0);

#define AFLOAT_SIGN_NEG 0
#define AFLOAT_SIGN_POS 1

typedef struct {
	char sign;
	ARR *d;
} AFLOAT;

void afloat_init(void);
void afloat_destroy(void);
void afloat_print(const AFLOAT *ptr);
ARR *afloat_getd(const AFLOAT *ptr);
size_t afloat_getd_len(const AFLOAT *ptr);
int afloat_setd_str(char *digits, AFLOAT *ptr);
int afloat_setd(char *digits, size_t len, AFLOAT *ptr);
void afloat_cleard(AFLOAT *ptr);
AFLOAT *afloat_define(void);
void afloat_dump_all(void);
void afloat_free(AFLOAT *ptr);
void afloat_free_all(void);

#endif
