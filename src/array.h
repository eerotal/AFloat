#ifndef INCLUDED_ARRAY_
#define INCLUDED_ARRAY_

#define AITEM_INT     0
#define AITEM_FLOAT   1
#define AITEM_DOUBLE  2
#define AITEM_CHAR    3
#define AITEM_PTR     4

typedef union {
	int i;
	float f;
	double d;
	char c;
	void *p;
} AITEM;

AITEM *array_put(AITEM *ptr, const AITEM *new_elem, const size_t len);
AITEM *array_pop(AITEM *ptr, const size_t index, const size_t len);
AITEM *array_pad(AITEM *ptr, const size_t old_len,
		const size_t new_len, const AITEM *filler,
		const int where);
AITEM *array_set(AITEM *ptr, const void *elems,
		const size_t len, int typ);

#endif
