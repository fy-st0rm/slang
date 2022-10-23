#include "sasm.h"
#include "cutil.h"
#include "tokenizer.h"

/* Print functions */
void usage() {
	printf("[USAGE]:\n");
	printf("    sasm [file.svm]\n");
	exit(1);
}

void print_tokenizer_src(Tokenizer* tokenizer) {
	printf("------TOKENIZER SOURCE START-------\n");
	printf("%s\n", tokenizer->source.c_str);
	printf("------TOKENIZER SOURCE END  -------\n");
}

void print_token(Token* token) {
	printf("--Token:\n");
	printf("  kind: %s\n", token_kind_to_str(token->kind));
	printf("  value: %s\n", token->source.c_str);
	printf("  Loc: %s:%d:%d\n", token->loc.file, token->loc.row, token->loc.col);
}

void print_sasm_memory(Sasm* sasm) {
	printf("-------SASM MEMORY START----------\n");
	for (int i = 0; i < sasm->mc; i++) {
		printf("%d\n", sasm->memory[i]);
	}
	printf("-------SASM MEMORY END  ----------\n");
}

/* Sasm functions */
void __extend_sasm_memory(Sasm* sasm) {
	uint64_t tmp[sasm->memory_sz];
	memcpy(tmp, sasm->memory, sizeof(int64_t) * sasm->memory_sz);
	cutil_free(sasm->memory);
	sasm->memory_sz *= 2;
	sasm->memory = cutil_alloc(sizeof(int64_t) * sasm->memory_sz);
	memcpy(sasm->memory, tmp, sizeof(int64_t)  * sasm->memory_sz);
}

void __extend_sasm_program(Sasm* sasm) {
	Svm_Opcode tmp[sasm->program_sz];
	memcpy(tmp, sasm->program, sizeof(Svm_Opcode) * sasm->program_sz);
	cutil_free(sasm->program);
	sasm->program_sz *= 2;
	sasm->program = cutil_alloc(sizeof(Svm_Opcode) * sasm->program_sz);
	memcpy(sasm->program, tmp, sizeof(Svm_Opcode)  * sasm->program_sz);
}

Sasm sasm_init(const char* file_name, uint64_t memory_sz, uint64_t program_sz) {
	Sasm sasm = {
		.file_name = cutil_str_new(file_name),
		.memory = cutil_alloc(sizeof(int64_t) * memory_sz),
		.memory_sz = memory_sz,
		.mc = 0,
		.program = cutil_alloc(sizeof(Svm_Opcode) * program_sz),
		.program_sz = program_sz,
		.pc = 0
	};
	return sasm;
}

void sasm_delete(Sasm* sasm) {
	//FIXME: Freeing memory causes core dump in hello world example
	cutil_str_delete(&sasm->file_name);
	cutil_free(sasm->memory);
	cutil_free(sasm->program);
}

void sasm_push_memory(Sasm* sasm, int64_t mem) {
	if (sasm->mc >= sasm->memory_sz) {
		__extend_sasm_memory(sasm);
	}
	sasm->memory[sasm->mc++] = mem;
}

void sasm_push_opcode(Sasm* sasm, Svm_Opcode code) {
	if (sasm->pc >= sasm->program_sz) {
		__extend_sasm_program(sasm);
	}
	sasm->program[sasm->pc++] = code;
}

void sasm_gen_svm_bytecode(Sasm* sasm) {
	cutil_str_add_c_str(&sasm->file_name, ".svm");
	FILE* fd = fopen(sasm->file_name.c_str, "wb");
	if (!fd) {
		cutil_assert(false, "Failed to write to file: %s\n", sasm->file_name.c_str);
	}

	for (int i = 0; i < META_SIZE; i++) {
		fwrite(&sasm->meta[i], 1, 1, fd);
	}

	for (int i = 0; i < sasm->mc; i++) {
		fwrite(&sasm->memory[i], 1, 1, fd);
	}

	for (int i = 0; i < sasm->pc; i++) {
		fwrite(&sasm->program[i], 1, 1, fd);
	}

	fclose(fd);
}

/* Execute function */
void execute_tokens(Sasm* sasm, Tokenizer* tokenizer) {
	while (tokenizer->source.len || tokenizer->line.len) {
		Token token = get_next_token(tokenizer);
		//print_token(&token);

		switch (token.kind) {
			case TOKEN_KIND_INST: {
				Sasm_Inst inst = str_to_inst(token.source.c_str);
				switch (inst) {
					case INST_PUSHI: {
						Token value = expect_next_token(tokenizer, TOKEN_KIND_INT);
						sasm_push_memory(sasm, value.value.as_i64);
						sasm_push_opcode(sasm, OP_PUSH_INT);
						break;
					}
					case INST_PUSHS: {
						Token value = expect_next_token(tokenizer, TOKEN_KIND_STR);
						int len = strlen(value.value.as_str);
						sasm_push_memory(sasm, len);
						for (int i = 0; i < len; i++) {
							sasm_push_memory(sasm, value.value.as_str[i]);
						}
						sasm_push_opcode(sasm, OP_PUSH_STR);
						break;
					}
					case INST_PUSHC: {
						Token value = expect_next_token(tokenizer, TOKEN_KIND_CHAR);
						sasm_push_memory(sasm, value.value.as_char);
						sasm_push_opcode(sasm, OP_PUSH_CHAR);
						break;
					}
					case INST_PRINTI: {
						sasm_push_opcode(sasm, OP_PRINT_INT);
						break;
					}
					case INST_PRINTS: {
						sasm_push_opcode(sasm, OP_PRINT_STR);
						break;
					}
					case INST_PRINTC: {
						sasm_push_opcode(sasm, OP_PRINT_CHAR);
						break;
					}
					case INST_ADDI: {
						if (sasm->mc < 2) {
							error(token.loc, "ADDI instruction requires 2 operand.\n");
						}
						sasm_push_opcode(sasm, OP_ADD_INT);
						break;
					}
					default:
						cutil_assert(false, "Unhandled instruction `%s`\n", inst_to_str(inst));
				}
				break;
			}
			case TOKEN_KIND_INT: {
				cutil_assert(false, "INT not implemented\n");
				break;
			}
			default:
				cutil_assert(false, "Unknown token kind `%d`\n", token.kind);
		}
	}
	// Writing the value to meta
	sasm->meta[0] = sasm->mc;
	sasm->meta[1] = sasm->pc;
}

int main(int argc, char** argv) {
	if (argc <= 1) usage();

	Sasm sasm = sasm_init(argv[1], MEMORY_CAPACITY, PROGRAM_CAPACITY);
	Tokenizer tokenizer = tokenizer_from_file(argv[1]);
	execute_tokens(&sasm, &tokenizer);
	sasm_gen_svm_bytecode(&sasm);

	//sasm_delete(&sasm);
	cutil_alert();
	return 0;
}
