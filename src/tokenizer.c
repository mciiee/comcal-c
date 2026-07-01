#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "list.h"
#include "token.h"

#ifndef TOKENLIST_DEFAULT_CAPACITY 
#define TOKENLIST_DEFAULT_CAPACITY 256
#endif

#ifndef VAR_STRING_DEFAULT_LENGTH 
#define VAR_STRING_DEFAULT_LENGTH 4
#endif

constexpr Token TOKEN_EOF = {
  .flags = 0,
  .type = TOKENTYPE_EOF,
  .repr.str = nullptr
};

constexpr Token TOKEN_WHITESPACE = {
  .flags = 0,
  .type = TOKENTYPE_WHITESPACE,
  .repr.str = nullptr,
};

constexpr Token TOKEN_NEWLINE = {
  .flags = 0,
  .type = TOKENTYPE_NEWLINE,
  .repr.str = nullptr,
};

constexpr Token TOKEN_COMBINATOR_S = {
  .flags = 0,
  .type = TOKENTYPE_COMBINATOR,
  .repr.combinator = COMBINATOR_S
};

constexpr Token TOKEN_COMBINATOR_K = {
  .flags = 0,
  .type = TOKENTYPE_COMBINATOR,
  .repr.combinator = COMBINATOR_K
}
;
constexpr Token TOKEN_COMBINATOR_I = {
  .flags = 0,
  .type = TOKENTYPE_COMBINATOR,
  .repr.combinator = COMBINATOR_I
};

constexpr Token TOKEN_PAREN_OPEN = {
  .flags = 0,
  .type = TOKENTYPE_PAREN_OPEN,
};

constexpr Token TOKEN_PAREN_CLOSE = {
  .flags = 0,
  .type = TOKENTYPE_PAREN_CLOSE,
};



static char *handleVar(FILE *stream) {
  char *str = calloc(VAR_STRING_DEFAULT_LENGTH+1, sizeof(char));
  size_t len = VAR_STRING_DEFAULT_LENGTH;

  size_t i = 0;
  char c = getc(stream);
  while (!feof(stream) && islower(c)) {
    if (i > len) {
      len *= 1.5;
      str = realloc(str, sizeof(char) * len);
    }
    str[i] = c;
    i++;

    c = fgetc(stream);
  }
  str[i] = '\0';
  ungetc(c, stream);

  return str;
}

TokenList *tokenize(FILE *stream) {
  TokenList *list = TokenList_new(TOKENLIST_DEFAULT_CAPACITY);
  TokenList *list_err = nullptr;
  (void)list_err;
  Token token = {0};
  char c = '\0';

  while (!feof(stream)) {
    c = fgetc(stream);
    switch (c) {
      case ' ':
        [[fallthrough]];
      case '\t':
        [[fallthrough]];
      case '\r':
        // _TODO: Add error checking
        list_err = TokenList_append(list, &TOKEN_WHITESPACE);
        break;
      case '\n':
        // _TODO: Add error checking
        list_err = TokenList_append(list, &TOKEN_NEWLINE);
        break;
      case EOF:
        list_err = TokenList_append(list, &TOKEN_EOF);
        break;

      case '(':
        list_err = TokenList_append(list, &TOKEN_PAREN_OPEN);
        break;
      case ')':
        list_err = TokenList_append(list, &TOKEN_PAREN_CLOSE);
        break;

      case 'S':
        list_err = TokenList_append(list, &TOKEN_COMBINATOR_S);
        break;
      case 'I':
        list_err = TokenList_append(list, &TOKEN_COMBINATOR_I);
        break;
      case 'K':
        list_err = TokenList_append(list, &TOKEN_COMBINATOR_K);
        break;
      default:
        if (islower(c)) {
          ungetc(c, stream);
          token.type = TOKENTYPE_VAR;
          token.flags = TOKENFLAGS_REPR_DYNAMICALLY_ALLOCATED;
          token.repr.str = handleVar(stream);

          list_err = TokenList_append(list, &token);
          break;
        }
        token.type = TOKENTYPE_UNKNOWN;
        token.flags = TOKENFLAGS_REPR_DYNAMICALLY_ALLOCATED;
        token.repr.str = malloc(2 * sizeof(char));
        token.repr.str[0] = c;
        token.repr.str[1] = '\0';

        list_err = TokenList_append(list, &token);
        break;
    }
  }


  return list;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  FILE *file = stdin;
  //FILE *file = fopen("test/test", "r");
  //if (file == nullptr) {
  //  fprintf(stderr, "File :( \n");
  //  return 1;
  //}

  auto list = tokenize(file);
  TokenList_print(list);


  //fclose(file);
  return 0;
}
