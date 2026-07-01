#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "list.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


TokenList *tokenize(FILE *stream);
#endif
