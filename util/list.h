#ifndef LIST_H
#define LIST_H

#include "tokenizer.h"

typedef struct TokenList {
  size_t capacity;
  size_t occupied;
  struct Token *tokens;
} TokenList;

[[nodiscard("Dynamically allocated")]]
TokenList *TokenList_new(size_t capacity);
[[nodiscard("Check for error")]]
TokenList *TokenList_append(TokenList *list, const struct Token *token);
void TokenList_free(TokenList *list);
#endif
