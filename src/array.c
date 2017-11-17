#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"

int array_set(ARR *ptr, const void *elems, const size_t len, int typ) {
	AITEM tmp;
	for (size_t i = 0; i < len; i++) {
		memset(&tmp, 0, sizeof(tmp));
		switch(typ) {
			case AITEM_INT:
				tmp.i = ((int*) elems)[i];
				break;
			case AITEM_FLOAT:
				tmp.f = ((float*) elems)[i];
				break;
			case AITEM_DOUBLE:
				tmp.d = ((double*) elems)[i];
				break;
			case AITEM_CHAR:
				tmp.c = ((char*) elems)[i];
				break;
			case AITEM_PTR:
				tmp.p = ((char**) elems)[i];
				break;
			default:
				break;
		}
		if (!array_put(ptr, &tmp)) {
			return -1;
		}
	}
	return 1;
}

int array_put(ARR *ptr, const AITEM *new_elem) {
	/*
	*  Append the element 'new_elem' to the array 'ptr'.
	*/
	AITEM *tmp = NULL;
	tmp = realloc(ptr->elems, (ptr->len + 1)*sizeof(*new_elem));
	if (!tmp) {
		return -1;
	}
	ptr->elems = tmp;
	ptr->len++;

	if (new_elem) {
		memcpy(ptr->elems + ptr->len - 1, new_elem,
			sizeof(*new_elem));
	} else {
		memset(ptr->elems + ptr->len - 1, 0, sizeof(*new_elem));
	}
	return 1;
}

int array_pop(ARR *ptr, const size_t index) {
	AITEM *tmp = NULL;
	size_t tmp_index = 0;

	if (!ptr) {
		return -1;
	}

	tmp = malloc((ptr->len - 1)*sizeof(AITEM));
	if (!tmp) {
		return -1;
	}

	for (size_t i = 0; i < ptr->len; i++) {
		if (i != index) {
			memcpy(tmp + tmp_index, ptr->elems + i,
				sizeof(AITEM));
			tmp_index++;
		}
	}
	free(ptr->elems);
	ptr->elems = tmp;
	ptr->len--;
	return 1;
}

int array_pad(ARR *ptr, const size_t new_len,
		const AITEM *filler, const int where) {
	AITEM *tmp = NULL;

	if (!ptr) {
		return -1;
	}

	if (ptr->len > new_len) {
		return -1;
	}

	if (ptr->len == new_len) {
		return 1;
	}

	tmp = malloc(new_len*sizeof(*ptr->elems));
	if (!tmp) {
		return -1;
	}

	if (where == -1) {
		memcpy(tmp + new_len - ptr->len, ptr->elems,
			ptr->len*sizeof(*ptr->elems));
		for (size_t i = 0; i <= new_len - ptr->len - 1; i++) {
			memcpy(tmp + i, filler, sizeof(*filler));
		}
	} else if (where == 1) {
		memcpy(tmp, ptr->elems, ptr->len*sizeof(*ptr->elems));
		for (size_t i = 0; i <= new_len - ptr->len - 1; i++) {
			memcpy(tmp + ptr->len + i, filler,
				sizeof(*filler));
		}
	} else {
		free(tmp);
		return -1;
	}

	free(ptr->elems);
	ptr->elems = tmp;
	ptr->len = new_len;
	return 1;
}
