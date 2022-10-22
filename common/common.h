#ifndef _COMMON_H_
#define _COMMON_H_

#include "log.h"

static int get_file_size(FILE* file) {
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

#endif
