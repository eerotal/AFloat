#include <stdio.h>

#include "afloat.h"
#include "afloat_discrete.h"
#include "afloat_math.h"

int main(int argc, const char **argv) {
	printf("ArbFloat test utility.\n");

	AFLOAT *test1 = afloat_define();
	AFLOAT *test2 = afloat_define();

	afloat_setd_str("1234567890", test1);
	printf("test1: ");
	afloat_print(test1);
	printf("\n");

	afloat_setd_str("9876", test2);
	printf("test2: ");
	afloat_print(test2);
	printf("\n");
	afloat_pad(test2, test1);
	printf("test2: ");
	afloat_print(test2);
	printf("\n");


	afloat_dump_all();
	afloat_free_all();
}