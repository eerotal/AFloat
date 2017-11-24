#ifndef INCLUDED_ALOG
#define INCLUDED_ALOG

#include <stdio.h>
#include <string.h>

extern int print_verbose;

#define ALOG_NEWLINE '\n'

#define alog_printm(...)						\
	_alog_printm(__VA_ARGS__, ALOG_NEWLINE)
#define _alog_printm(s, ...)						\
	printf(": %s: "s"%c", __FILE__, ##__VA_ARGS__);			\

#define alog_printv(...)						\
	_alog_printv(__VA_ARGS__, ALOG_NEWLINE)
#define _alog_printv(s, ...)						\
	if (print_verbose >= 1) {					\
		printf(": %s: "s"%c", __FILE__, ##__VA_ARGS__);		\
	}

#define alog_printvv(...)						\
	_alog_printvv(__VA_ARGS__, ALOG_NEWLINE)
#define _alog_printvv(s, ...)						\
	if (print_verbose >= 2) {					\
		printf(": %s: "s"%c", __FILE__, ##__VA_ARGS__);		\
	}

void alog_set_verbose(int flag);

#endif
