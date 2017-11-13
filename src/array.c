#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"

AITEM *array_set(AITEM *ptr, const void *elems,
		const size_t len, int typ) {
	/*
	*  Copy the elements of the traditional C-style array
	*  'elems' into the AITEM array 'ptr'. 'len' is the length
	*  of the original array and 'typ' should be one of the AITEM_*
	*  constants defined in array.h. The 'typ' argument is used
	*  to tell this function the data type that the original array
	*  contains. Returns an AITEM array pointer on success or NULL
	*  on failure.
	*/

	AITEM *tmp_arr = NULL;
	AITEM *arr = NULL;
	size_t arr_len = 0;
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
		tmp_arr = array_put(arr, &tmp, arr_len);
		if (!tmp_arr) {
			if (arr) {
				free(arr);
			}
			return NULL;
		}
		arr = tmp_arr;
		arr_len++;
	}
	return arr;
}

AITEM *array_put(AITEM *ptr, const AITEM *new_elem,
		const size_t len) {
	/*
	*  Append the element 'new_elem' to the AITEM array 'ptr'.
	*  'len' is the current length of the AITEM array.
	*  Returns a pointer to a new AITEM array on success or
	*  NULL on failure.
	*/

	AITEM *tmp = NULL;

	printf("Array: put *(0x%x) --> *(0x%x)(start).\n",
		(unsigned int) new_elem,
		(unsigned int) ptr);

	tmp = realloc(ptr, (len + 1)*sizeof(*new_elem));
	if (!tmp) {
		return NULL;
	}

	printf("Array: realloc'd %zu bytes @ 0x%x.\n",
		(len + 1)*sizeof(new_elem),
		(unsigned int) tmp);

	if (new_elem) {
		memcpy(tmp + len, new_elem, sizeof(*new_elem));
	} else {
		memset(tmp + len, 0, sizeof(*new_elem));
	}
	return tmp;
}

AITEM *array_pop(AITEM *ptr, const size_t index,
		const size_t len) {
	/*
	*  Remove the item at 'index' in the AITEM array 'ptr'.
	*  'len' is the current length of the array. Returns a
	*  new AITEM array pointer on success or NULL on failure.
	*/

	AITEM *tmp = NULL;
	size_t tmp_index = 0;

	if (!ptr || !len) {
		return NULL;
	}

	tmp = malloc((len - 1)*sizeof(AITEM));
	if (!tmp) {
		return NULL;
	}

	for (size_t i = 0; i < len; i++) {
		if (i != index) {
			memcpy(tmp + tmp_index, ptr + i,
				sizeof(AITEM));
			tmp_index++;
		}
	}
	free(ptr);
	return tmp;
}

AITEM *array_pad(AITEM *ptr, const size_t old_len,
		const size_t new_len, const AITEM *filler,
		const int where) {
	/*
	*  Pad the AITEM array 'ptr' with 'filler' so that the new
	*  length of the array is equal to 'new_len'. 'old_len' is
	*  the length of the array before this operation is done.
	*  'where' is used to specify where to append 'filler'. -1
	*  appends 'filler' at the beginning of the array and 1 at
	*  the end of the array. Returns a pointer to a new AITEM
	*  array on success or a NULL pointer on failure.
	*/

	AITEM *tmp = NULL;

	if (!ptr) {
		return NULL;
	}

	if (old_len > new_len) {
		return NULL;
	}

	if (old_len == new_len) {
		return ptr;
	}

	tmp = malloc(new_len*sizeof(*ptr));
	if (!tmp) {
		return NULL;
	}

	if (where == -1) {
		memcpy(tmp + new_len - old_len, ptr,
			old_len*sizeof(*ptr));
		for (size_t i = 0; i <= new_len - old_len - 1; i++) {
			memcpy(tmp + i, filler, sizeof(*filler));
		}
	} else if (where == 1) {
		memcpy(tmp, ptr, old_len*sizeof(*ptr));
		for (size_t i = 0; i <= new_len - old_len - 1; i++) {
			memcpy(tmp + old_len + i, filler,
				sizeof(*filler));
		}
	} else {
		free(tmp);
		return NULL;
	}

	free(ptr);
	return tmp;
}
