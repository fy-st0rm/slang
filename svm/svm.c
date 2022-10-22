#include "cutil.h"
#include "svm.h"

int get_file_sz(FILE* file) {
	fseek(file, 0, SEEK_END);
	int sz = ftell(file);
	fseek(file, 0, SEEK_SET);
	return sz;
}

/* Print functions */
void print_meta(Svm_Meta* meta) {
	printf("------PROGRAM META START---------\n");
	printf("- stack size: %d\n", meta->stack_size);
	printf("- program size: %d\n", meta->program_size);
	printf("------PROGRAM META END---------\n");
}

void print_stack(Svm* svm) {
	printf("------PROGRAM STACK START---------\n");
	for (int i = 0; i < svm->stack_size; i++) {
		printf("%d ", svm->stack[i].as_i64);
	}
	printf("\n");
	printf("------PROGRAM STACK END---------\n");
}
void print_program(Svm* svm) {
	printf("------PROGRAM STACK START---------\n");
	for (int i = 0; i < svm->program_size; i++) {
		printf("%d ", svm->program[i]);
	}
	printf("\n");
	printf("------PROGRAM STACK END---------\n");
}

/* Program loaders */

Svm_Meta load_meta(const char** buffer) {
	Svm_Meta meta = {0};
	meta.stack_size = *(*buffer)++;
	meta.program_size = *(*buffer)++;
	return meta;
}

void load_stack(Svm* svm, const char** buffer) {
	memset(svm->stack, 0, sizeof(svm->stack));

	for (int i = 0; i < svm->stack_size; i++) {
		char ch = *(*buffer)++;
		Svm_Value val = {
			.as_ui64 = ch,
			.as_i64  = ch,
			.as_char = ch,
			.as_str  = ch
		};
		svm->stack[i] = val;
	}
}

void load_program(Svm* svm, const char** buffer) {
	memset(svm->program, 0, sizeof(svm->program));

	for (int i = 0; i < svm->program_size; i++) {
		char ch = *(*buffer)++;
		svm->program[i] = ch;
	}
}

void load_bytecode(Svm* svm, const char* file_name) {
	FILE* fd = fopen(file_name, "rb");
	if (!fd) {
		cutil_assert(false, "Failed to open file `%s`.\n", file_name);
	}

	// Reading the file
	int size = get_file_sz(fd);
	const char* buffer = cutil_alloc(size);
	fread(buffer, sizeof(buffer), 1, fd);

	// Loading meta data
	Svm_Meta meta = load_meta(&buffer);
	svm->stack_size = meta.stack_size;
	svm->program_size = meta.program_size;
	print_meta(&meta);

	// Loading the stack
	load_stack(svm, &buffer);

	// Loading the program
	load_program(svm, &buffer);

	cutil_free(buffer);
	fclose(fd);
}

void usage() {
	printf("[USAGE]:\n");
	printf("    svm [file.svm]\n");
	exit(1);
}

int main(int argc, char** argv) {
	if (argc <= 1) usage();

	Svm svm = {0};
	load_bytecode(&svm, argv[1]);

	print_stack(&svm);
	print_program(&svm);

	cutil_alert();
	return 0;
}
