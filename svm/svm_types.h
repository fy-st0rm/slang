#ifndef _OPCODES_H_
#define _OPCODES_H_

#include "../cutil/cutil.h"
#include <stdint.h>

typedef enum {
	TP_INT,
	TP_CHAR,
	TP_STR
} Svm_Type;

typedef enum {
	OP_PUSH_INT,
	OP_PUSH_STR,
	OP_PUSH_CHAR,
	OP_PRINT_INT,
	OP_PRINT_STR,
	OP_PRINT_CHAR,
	OP_ADD_INT
} Svm_Opcode;

typedef struct {
	uint64_t as_ui64;
	int64_t  as_i64;
	char     as_char;
	char*    as_str;
} Svm_Value;

typedef struct {
	Svm_Value value;
	Svm_Type type;
} Svm_Stack_Value;

typedef struct {
	uint64_t memory_size;
	uint64_t program_size;
} Svm_Meta;

static char* opcode_to_str(uint64_t opcode) {
	switch (opcode) {
		case OP_PUSH_INT:
			return "OP_PUSH_INT";
		case OP_PUSH_STR:
			return "OP_PUSH_STR";
		case OP_PUSH_CHAR:
			return "OP_PUSH_CHAR";
		case OP_PRINT_INT:
			return "OP_PRINT_INT";
		case OP_PRINT_STR:
			return "OP_PRINT_STR";
		case OP_PRINT_CHAR:
			return "OP_PRINT_CHAR";
		case OP_ADD_INT:
			return "OP_ADD_INT";
		default:
			cutil_assert(false, "Unknown opcode `%d`\n", opcode);
	}
}

static char* type_to_str(uint64_t type) {
	switch (type) {
		case TP_INT:
			return "TP_INT";
		case TP_CHAR:
			return "TP_CHAR";
		case TP_STR:
			return "TP_STR";
		default:
			cutil_assert(false, "Unknown type with code `%d`\n", type);
	}
}

#endif
