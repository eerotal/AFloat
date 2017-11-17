#include <stdio.h>

#include "afloat.h"
#include "afloat_discrete.h"
#include "afloat_math.h"
#include "alog.h"

int main(int argc, const char **argv) {
	printf("ArbFloat test utility.\n");

	alog_set_verbose(2);

	afloat_init();

	AFLOAT *test1 = afloat_define();
	AFLOAT *test2 = afloat_define();
	AFLOAT *test_sum = NULL;

	afloat_setd_str("1234567890", test1);
	printf("test1: ");
	afloat_print(test1);
	printf("\n");

	afloat_setd_str("9876", test2);
	printf("test2: ");
	afloat_print(test2);
	printf("\n");

	test_sum = afloat_sum(test1, test2);
	printf("test_sum: ");
	afloat_print(test_sum);
	printf("\n");

	afloat_dump_all();
	afloat_destroy();

}
