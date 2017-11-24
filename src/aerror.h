#ifndef INLCUDED_AERROR
#define INCLUDED_AERROR

enum AERROR {
	AERROR_OK               =  0,
	AERROR_INVALID_POINTER  = -1,
	AERROR_INVALID_ARGS     = -2,
	AERROR_MEMORY           = -3,
	AERROR_INTERNAL         = -4,
	AERROR_UNKNOWN          = -5
};

#define aerror_printerr(s)					\
	_aerror_printerr_custom(s, __LINE__, __FILE__)

#define aerror_printerr_num(err, force)				\
	 _aerror_printerr_num(err, force, __LINE__, __FILE__)

#define AERROR_FLG(f, flag)					\
	flag -= (f != AERROR_OK ? 1 : 0);

#define AERROR_FLGP(f, flag)					\
	flag -= (aerror_printerr_num(f, 0) != AERROR_OK ? 1 : 0)

#define AERROR_CHK(err)						\
	(err != AERROR_OK ? 0 : 1)

#define AERROR_CHKP(err)					\
	(aerror_printerr_num(err, 0) != AERROR_OK ? 0 : 1)

#define AERROR_CHKFLG(flag)					\
	(!flag)

char *aerror_strerr(enum AERROR err);
enum AERROR _aerror_printerr_num(enum AERROR err, int force,
				int ln, char *f);
void _aerror_printerr_custom(char *s, int ln, char *f);

#endif

