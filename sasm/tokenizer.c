#include "tokenizer.h"

char* inst_to_str(Sasm_Inst inst) {
	switch (inst) {
		case INST_PUSHI:
			return "pushi";
		case INST_PUSHS:
			return "pushs";
		case INST_PRINTI:
			return "printi";
		case INST_PRINTS:
			return "prints";
		case INST_ADDI:
			return "addi";
		default:
			cutil_assert(false, "Unknown instruction with code `%d`\n", inst);
	}
}

Sasm_Inst str_to_inst(char* c_str) {
	if (!strcmp(c_str, inst_to_str(INST_PUSHI)))
		return INST_PUSHI;
	if (!strcmp(c_str, inst_to_str(INST_PUSHS)))
		return INST_PUSHS;
	else if (!strcmp(c_str, inst_to_str(INST_PRINTI)))
		return INST_PRINTI;
	else if (!strcmp(c_str, inst_to_str(INST_PRINTS)))
		return INST_PRINTS;
	else if (!strcmp(c_str, inst_to_str(INST_ADDI)))
		return INST_ADDI;
	cutil_assert(false, "Unknown instruction with name `%s`\n", c_str);
}

char* token_kind_to_str(uint64_t token_kind) {
	switch (token_kind) {
		case TOKEN_KIND_INT:
			return "TOKEN_KIND_INT";
		case TOKEN_KIND_STR:
			return "TOKEN_KIND_STR";
		case TOKEN_KIND_INST:
			return "TOKEN_KIND_INST";
		default:
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
	if (!strcmp(token_value.c_str, inst_to_str(INST_PUSHS))) {
		return TOKEN_KIND_INST;
	}
	else if (!strcmp(token_value.c_str, inst_to_str(INST_PRINTI))) {
		return TOKEN_KIND_INST;
	}
	else if (!strcmp(token_value.c_str, inst_to_str(INST_PRINTS))) {
		return TOKEN_KIND_INST;
	}
	else if (!strcmp(token_value.c_str, inst_to_str(INST_ADDI))) {
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

	str word;
	Token_Kind tk;

	if (tokenizer->line.c_str[0] != '\"') {
		word = cutil_str_split(&tokenizer->line, ' ');
		tk = identify_token(word);
	} else {
		// Parsing the string

		word = cutil_str_new("");
		char i = *tokenizer->line.c_str++;
		tokenizer->line.len--;
		do {
			i = *tokenizer->line.c_str++;
			tokenizer->line.len--;
			if (i != '\"') cutil_str_add_char(&word, i);
		} while (i != '\"');

		tk = TOKEN_KIND_STR;
	}

	// Creating the value
	Svm_Value value = {
		.as_ui64 = (uint64_t) atoi(word.c_str),
		.as_i64  = (int64_t)  atoi(word.c_str),
		.as_char = (char)     *word.c_str,
	};
	if (tk == TOKEN_KIND_STR) {
		value.as_str = cutil_alloc(strlen(word.c_str));
		strcpy(value.as_str, word.c_str);
	}

	// Creating the token
	Token token = {
		.kind   = tk,
		.value  = value,
		.source = cutil_str_new(word.c_str),
		.loc    = tokenizer->curr_loc
	};

	tokenizer->curr_loc.col += word.len;
	if (token.kind == TOKEN_KIND_STR)
		tokenizer->curr_loc.col += 2;

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
