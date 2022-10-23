#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "cutil.h"
#include <stdint.h>
#include "common.h"
#include "../svm/svm_types.h"

typedef enum {
	INST_PUSHI,
	INST_PUSHS,
	INST_PRINTI,
	INST_PRINTS,
	INST_ADDI
} Sasm_Inst;

char* inst_to_str(Sasm_Inst inst);
Sasm_Inst str_to_inst(char* c_str);

typedef enum {
	TOKEN_KIND_INT,
	TOKEN_KIND_STR,
	TOKEN_KIND_INST
} Token_Kind;

char* token_kind_to_str(uint64_t token_kind);

typedef struct {
	const char* file;
	uint64_t row, col;
} Location;

typedef struct {
	Token_Kind kind;
	Svm_Value value;
	str source;
	Location loc;
} Token;

typedef struct {
	const char* file;
	str source;
	str line;
	Location curr_loc;
} Tokenizer;

Tokenizer tokenizer_from_file(const char* file);
Token_Kind identify_token(str token_value);
char get_escape_code(char ch);
Token get_next_token(Tokenizer* tokenizer);
Token expect_next_token(Tokenizer* tokenizer, Token_Kind token_kind);

#endif
