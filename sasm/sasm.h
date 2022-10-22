#ifndef _SASM_H_
#define _SASM_H_

#include "tokenizer.h"

/*
 * Should include:
 * Meta data
 * Memory
 * Program instruction
 */

typedef struct {
} Sasm;

void execute_tokens(Tokenizer* tokenizer);

#endif
