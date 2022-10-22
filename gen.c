#include <stdio.h>
#include "svm/svm_types.h"
#include <stdint.h>

int main() {
	int meta_size = 2;
	int mem_size = 1;
	int program_size = 2;
	int meta[] = {
		mem_size, program_size
	};

	uint64_t memory[] = {
		10
	};

	int program[] = { 
		OP_PUSH_INT,
		OP_PRINT_INT
	};

	FILE* file = fopen("test.svm", "wb");

	for (int i = 0; i < meta_size; i++)
		fwrite(&meta[i], 1, 1, file);

	for (int i = 0; i < mem_size; i++)
		fwrite(&memory[i], 1, 1, file);

	for (int i = 0; i < program_size; i++)
		fwrite(&program[i], 1, 1, file);

	fclose(file);
}
