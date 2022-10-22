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
	printf("  value: %s\n", token->value.c_str);
	printf("  Loc: %s:%d:%d\n", token->loc.file, token->loc.row, token->loc.col);
}


/* Execute function */
void execute_tokens(Tokenizer* tokenizer) {
	while (tokenizer->source.len) {
		Token token = get_next_token(tokenizer);

		switch (token.kind) {
			case TOKEN_KIND_INST: {
				Sasm_Inst inst = str_to_inst(token.value.c_str);
				switch (inst) {
					case INST_PUSHI: {
						Token value = expect_next_token(tokenizer, TOKEN_KIND_INT);
						print_token(&value);
						cutil_assert(false, "PUSHI not implemented\n");
					}
					case INST_PRINTI: {
						cutil_assert(false, "PRINTI not implemented\n");
					}
				}
			}
			case TOKEN_KIND_INT: {
				cutil_assert(false, "INT not implemented\n");
			}
			default:
				cutil_assert(false, "Unknown token kind `%d`\n", token.kind);
		}
	}
}

int main(int argc, char** argv) {
	if (argc <= 1) usage();
	Tokenizer tokenizer = tokenizer_from_file(argv[1]);
	execute_tokens(&tokenizer);
	cutil_alert();
	return 0;
}
