#ifndef _OPCODES_H_
#define _OPCODES_H_

#include "../cutil/cutil.h"
#include <stdint.h>

typedef enum {
	TP_INT
} svm_types;

typedef enum {
	OP_PUSH_INT,
	OP_POP_INT,
	OP_PRINT_INT
} svm_opcodes;

static char* opcode_to_str(uint64_t opcode) {
	switch (opcode) {
		case OP_PUSH_INT:
			return "OP_PUSH_INT";
		case OP_POP_INT:
			return "OP_POP_INT";
		case OP_PRINT_INT:
			return "OP_PRINT_INT";
		default:
			cutil_assert(false, "Unknown opcode `%d`\n", opcode);
	}
}

static char* type_to_str(uint64_t type) {
	switch (type) {
		case TP_INT:
			return "TP_INT";
		default:
			cutil_assert(false, "Unknown type with code `%d`\n", type);
	}
}

#endif
