#include <stdio.h>
#include "svm/svm_opcodes.h"
#include <stdint.h>

int main() {
	int stack_size = 3;
	int program_size = 2;
	int meta[] = {
		stack_size, program_size
	};

	uint64_t stack[] = {
		10, 20, 30
	};

	int program[] = { 
		OP_PUSH,
		OP_PRINT_INT
	};

	FILE* file = fopen("test.svm", "wb");

	for (int i = 0; i < 2; i++)
		fwrite(&meta[i], 1, 1, file);

	for (int i = 0; i < stack_size; i++)
		fwrite(&stack[i], 1, 1, file);

	for (int i = 0; i < program_size; i++)
		fwrite(&program[i], 1, 1, file);

	fclose(file);
}
