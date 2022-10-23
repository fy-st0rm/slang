#ifndef _SVM_H_
#define _SVM_H_

#include "svm_types.h"
#include "cutil.h"
#include <stdint.h>
#include "common.h"

#define SVM_STACK_CAPACITY 1024
#define SVM_MEMORY_CAPACITY 1024
#define SVM_PROGRAM_CAPACITY 1024

typedef struct {
	Svm_Stack_Value stack[SVM_STACK_CAPACITY];
	uint64_t stack_size;

	Svm_Value memory[SVM_MEMORY_CAPACITY];
	uint64_t memory_size;
	uint64_t mp;

	Svm_Opcode program[SVM_PROGRAM_CAPACITY];
	uint64_t program_size;
} Svm;

/* Print functions */
void print_meta(Svm_Meta* meta);
void print_stack(Svm* svm);
void print_memory(Svm* svm);
void print_program(Svm* svm);

/* Program loader */
void load_meta(Svm_Meta* meta, const char** buffer);
void load_memory(Svm* svm, const char** buffer);
void load_program(Svm* svm, const char** buffer);
void load_bytecode(Svm* svm, Svm_Meta* meta, const char* file_name);

/* Program executer */
void execute_program(Svm* svm);

#endif
