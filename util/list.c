#include "list.h"
#include <stdlib.h>

TokenList *TokenList_new(size_t capacity) {
  TokenList *list = malloc(sizeof(*list));
  list->tokens = malloc(sizeof(struct Token) * capacity);
  list->capacity = capacity;
  list->occupied = 0;

  return list;
}

static TokenList *TokenList_resize(TokenList *list, size_t new_capacity) {
  list->capacity = new_capacity;
  list->tokens = realloc(list->tokens, sizeof(struct Token) * new_capacity);
  if (list->tokens == nullptr) {
    return nullptr;
  }

  return list;
}

TokenList *TokenList_append(TokenList *list, const struct Token *token) {
  TokenList *list_copy = list;
  if (list->occupied >= list->capacity) {
    list_copy = TokenList_resize(list, list->capacity * 1.5);
  }
  if (list_copy == nullptr) {
    return nullptr;
  }

  list->tokens[list->occupied] = *token;
  list->occupied++;

  return list;
}


//TokenList *TokenList_create_append(TokenList *list, const struct Token *token) {}

void TokenList_free(TokenList *list) {
  free(list->tokens);
  free(list);
}
