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


typedef struct {
	AITEM *elems;
	size_t len;
	int typ;
} ARR;

AITEM *array_get(ARR *ptr, size_t index);
int array_put(ARR *ptr, const AITEM *new_elem);
int array_pop(ARR *ptr, const size_t index);
int array_pad(ARR *ptr, const size_t new_len,
		const AITEM *filler, const int where);
int array_set(ARR *ptr, const void *elems, const size_t len, int typ);

#endif
