#include "cutil.h"
#include "svm.h"

/* Print functions */
void print_buffer(const char* buffer, int size) {
	printf("-------BUFFER START---------\n");
	for (int i = 0; i < size; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n-------BUFFER END  ---------\n");
}

void print_meta(Svm_Meta* meta) {
	printf("------PROGRAM META START---------\n");
	printf("- memory size: %d\n", meta->memory_size);
	printf("- program size: %d\n", meta->program_size);
	printf("------PROGRAM META END---------\n");
}

void print_stack(Svm* svm) {
	printf("------PROGRAM STACK START---------\n");
	for (int i = 0; i < svm->stack_size; i++) {
		Svm_Stack_Value val = svm->stack[i];
		if (val.type == TP_INT) {
			printf("%d ", svm->stack[i].value.as_i64);
		}
		else {
			cutil_assert(false, "Unhandled type to print `%d`.", val.type);
		}
	}
	printf("\n");
	printf("------PROGRAM STACK END---------\n");
}

void print_memory(Svm* svm) {
	printf("------PROGRAM MEMORY START---------\n");
	for (int i = 0; i < svm->memory_size; i++) {
		printf("%d ", svm->memory[i].as_i64);
	}
	printf("\n");
	printf("------PROGRAM MEMORY END---------\n");
}

void print_program(Svm* svm) {
	printf("------PROGRAM START---------\n");
	for (int i = 0; i < svm->program_size; i++) {
		printf("%d ", svm->program[i]);
	}
	printf("\n");
	printf("------PROGRAM END---------\n");
}

/* Program loaders */

void load_meta(Svm_Meta* meta, const char** buffer) {
	meta->memory_size  = *(*buffer)++;
	meta->program_size = *(*buffer)++;
}

void load_memory(Svm* svm, const char** buffer) {
	memset(svm->memory, 0, sizeof(svm->memory));

	for (int i = 0; i < svm->memory_size; i++) {
		char ch = *(*buffer)++;
		Svm_Value val = {
			.as_ui64 = (uint64_t) ch,
			.as_i64  = (int64_t)  ch,
			.as_char = (char)     ch,
		};
		svm->memory[i] = val;
	}
}

void load_program(Svm* svm, const char** buffer) {
	memset(svm->program, 0, sizeof(svm->program));

	for (int i = 0; i < svm->program_size; i++) {
		char ch = *(*buffer)++;
		svm->program[i] = ch;
	}
}

void load_bytecode(Svm* svm, Svm_Meta* meta, const char* file_name) {
	FILE* fd = fopen(file_name, "rb");
	if (!fd) {
		cutil_assert(false, "Failed to open file `%s`.\n", file_name);
	}

	// Reading the file
	int size = get_file_size(fd);
	const char* buffer = cutil_alloc(size);
	fread(buffer, size, 1, fd);
	print_buffer(buffer, size);

	// Loading meta data
	load_meta(meta, &buffer);
	svm->mp = 0;
	svm->stack_size   = 0;
	svm->memory_size  = meta->memory_size;
	svm->program_size = meta->program_size;

	// Loading the stack
	load_memory(svm, &buffer);

	// Loading the program
	load_program(svm, &buffer);

	cutil_free(buffer);
	fclose(fd);
}

/* Program executer */

void execute_program(Svm* svm) {
	for (int i = 0; i < svm->program_size; i++) {
		uint64_t opcode = svm->program[i];
		switch (opcode) {
			case OP_PUSH_INT: {
				Svm_Stack_Value s_val = {
					.value = svm->memory[svm->mp++],
					.type  = TP_INT
				};
				svm->stack[svm->stack_size++] = s_val;
				break;
			}
			case OP_PUSH_STR: {
				Svm_Stack_Value str_len = {
					.value = svm->memory[svm->mp++],
					.type  = TP_STR
				};

				for (int i = 0; i < str_len.value.as_i64; i++) {
					Svm_Stack_Value ch = {
						.value = svm->memory[svm->mp++],
						.type  = TP_CHAR
					};
					svm->stack[svm->stack_size++] = ch;
				}

				svm->stack[svm->stack_size++] = str_len;
				break;
			}
			case OP_PRINT_INT: {
				Svm_Stack_Value s_val = svm->stack[--svm->stack_size];
				if (s_val.type == TP_INT) {
					printf("%d", s_val.value.as_i64);
				}
				else {
					cutil_assert(false, "%s cannot print type `%s`\n", opcode_to_str(opcode), type_to_str(s_val.type));
				}
				break;
			}
			case OP_PRINT_STR: {
				Svm_Stack_Value str_len = svm->stack[--svm->stack_size];
				if (str_len.type != TP_STR)
					cutil_assert(false, "%s cannot print type `%s`\n", opcode_to_str(opcode), type_to_str(str_len.type));

				uint64_t len = str_len.value.as_i64;

				char out[len];
				out[len] = '\0';

				// String in stack are stored in reversed so the insertion is reversed
				for (int i = len - 1; i >= 0; i--) {
					out[i] = svm->stack[--svm->stack_size].value.as_char;
				}

				printf("%s", out);
				break;
			}
			case OP_ADD_INT: {
				Svm_Stack_Value a = svm->stack[--svm->stack_size];
				Svm_Stack_Value b = svm->stack[--svm->stack_size];
				if (a.type == TP_INT && b.type == TP_INT) {
					Svm_Stack_Value res = a;
					res.value.as_i64 += b.value.as_i64;
					svm->stack[svm->stack_size++] = res;
				}
				else {
					cutil_assert(false, "%s cannot add type `%s` and `%s`\n", opcode_to_str(opcode), type_to_str(a.type), type_to_str(b.type));
				}
				break;
			}
			default:
				cutil_assert(false, "Unknown OPCODE `%d`\n", opcode);
		}
	}
}

void usage() {
	printf("[USAGE]:\n");
	printf("    svm [file.svm]\n");
	exit(1);
}

int main(int argc, char** argv) {
	if (argc <= 1) usage();

	Svm_Meta meta = {0};
	Svm svm = {0};
	load_bytecode(&svm, &meta, argv[1]);

	print_meta(&meta);
	print_memory(&svm);
	print_program(&svm);

	execute_program(&svm);

	cutil_alert();
	return 0;
}
