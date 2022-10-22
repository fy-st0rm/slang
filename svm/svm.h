#ifndef _SVM_H_
#define _SVM_H_

#include "svm_opcodes.h"
#include "cutil.h"
#include <stdint.h>

#define SVM_STACK_CAPACITY 1024
#define SVM_PROGRAM_CAPACITY 1024

typedef struct {
	uint64_t as_ui64;
	int64_t  as_i64;
	char     as_char;
	char*    as_str;
} Svm_Value;

typedef struct {
	uint64_t stack_size;
	uint64_t program_size;
} Svm_Meta;

typedef struct {
	Svm_Value stack[SVM_STACK_CAPACITY];
	uint64_t stack_size;

	svm_opcodes program[SVM_PROGRAM_CAPACITY];
	uint64_t program_size;
} Svm;


/* Utils */
int get_file_sz(FILE* file);

/* Print functions */
void print_meta(Svm_Meta* meta);
void print_stack(Svm* svm);
void print_program(Svm* svm);

/* Program loader */
Svm_Meta load_meta(const char** buffer);
void load_stack(Svm* svm, const char** buffer);
void load_program(Svm* svm, const char** buffer);
void load_bytecode(Svm* svm, const char* file_name);

#endif
