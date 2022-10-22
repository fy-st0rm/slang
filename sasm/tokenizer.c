#include "tokenizer.h"

char* inst_to_str(uint64_t inst) {
	switch (inst) {
		case INST_PUSHI:
			return "pushi";
		case INST_PRINTI:
			return "printi";
		default:
			cutil_assert(false, "Unknown instruction with code `%d`\n", inst);
	}
}

Sasm_Inst str_to_inst(char* c_str) {
	if (!strcmp(c_str, inst_to_str(INST_PUSHI)))
		return INST_PUSHI;
	else if (!strcmp(c_str, inst_to_str(INST_PRINTI)))
		return INST_PRINTI;
	cutil_assert(false, "Unknown instruction with name `%s`\n", c_str);
}

char* token_kind_to_str(uint64_t token_kind) {
	switch (token_kind) {
		case TOKEN_KIND_INT:
			return "TOKEN_KIND_INT";
		case TOKEN_KIND_INST:
			return "TOKEN_KIND_INST";
		defauly:
			cutil_assert(false, "Unknown token kind with code `%d`\n", token_kind);
	}
}

Tokenizer tokenizer_from_file(const char* file) {
	Location curr_loc = {
		.file = file,
		.row = 0,
		.col = 1
	};

	Tokenizer tokenizer = {
		.file = file,
		.source = cutil_str_new(""),
		.line = cutil_str_new(""),
		.curr_loc = curr_loc
	};

	// Reading the file
	FILE* fd = fopen(file, "r");
	if (!fd) {
		cutil_assert(false, "Failed to open file: %s\n", file);
	}

	int size = get_file_size(fd);
	cutil_str_extend(&tokenizer.source, size);
	fread(tokenizer.source.c_str, size, 1, fd);
	fclose(fd);
	return tokenizer;
}

Token_Kind identify_token(str token_value) {
	if (!strcmp(token_value.c_str, inst_to_str(INST_PUSHI))) {
		return TOKEN_KIND_INST;
	}
	else if (!strcmp(token_value.c_str, inst_to_str(INST_PRINTI))) {
		return TOKEN_KIND_INST;
	}
	else if (isdigit(*token_value.c_str)) { //TODO: This line only checks the first digit so fix this
		return TOKEN_KIND_INT;
	}
	else if (token_value.len > 0) {
		cutil_assert(false, "Unknown token value `%s`\n", token_value.c_str);
	}
}

Token get_next_token(Tokenizer* tokenizer) {
	if (!tokenizer->line.len) {
		cutil_str_delete(&tokenizer->line);
		tokenizer->line = cutil_str_split(&tokenizer->source, '\n');
		tokenizer->curr_loc.row++;
		tokenizer->curr_loc.col = 1;
	}

	str word = cutil_str_split(&tokenizer->line, ' ');
	word.c_str[word.len] = '\0';

	Token_Kind tk = identify_token(word);
	Token token = {
		.kind = tk,
		.value = cutil_str_new(word.c_str),
		.loc = tokenizer->curr_loc
	};
	tokenizer->curr_loc.col += word.len;

	cutil_str_delete(&word);
	return token;
}

Token expect_next_token(Tokenizer* tokenizer, Token_Kind token_kind) {
	Token token = get_next_token(tokenizer);
	if (token.kind != token_kind) {
		error(token.loc, "Expected token `%s` got `%s`.\n", token_kind_to_str(token_kind), token_kind_to_str(token.kind));

	}
	return token;
}
