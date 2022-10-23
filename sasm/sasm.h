#ifndef _SASM_H_
#define _SASM_H_

#include "tokenizer.h"

/*
 * Should include:
 * Meta data
 * Memory
 * Program instruction
 */

#define META_SIZE 2
#define MEMORY_CAPACITY  1024
#define PROGRAM_CAPACITY 1024

typedef struct {
	str file_name;
	uint64_t meta[META_SIZE];

	int64_t* memory;
	uint64_t memory_sz;
	uint64_t mc;

	Svm_Opcode* program;
	uint64_t program_sz;
	uint64_t pc;
} Sasm;

/* sasm functions */
Sasm sasm_init(const char* file_name, uint64_t memory_sz, uint64_t program_sz);
void sasm_delete(Sasm* sasm);
void sasm_push_memory(Sasm* sasm, int64_t mem);
void sasm_push_opcode(Sasm* sasm, Svm_Opcode code);
void sasm_gen_svm_bytecode(Sasm* sasm);

/* Execute function */
void execute_tokens(Sasm* sasm, Tokenizer* tokenizer);

#endif
