#ifndef _LOG_H_
#define _LOG_H_

#define error(loc, ...)\
	fprintf(stderr, "\033[31mError: %s:%d:%d:\033[0m ", loc.file, loc.row, loc.col);\
	fprintf(stderr, __VA_ARGS__);\
	exit(1)

#endif
