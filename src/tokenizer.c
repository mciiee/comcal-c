#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "list.h"

#ifndef TOKENLIST_DEFAULT_CAPACITY 
#define TOKENLIST_DEFAULT_CAPACITY 256
#endif

#ifndef VAR_STRING_DEFAULT_LENGTH 
#define VAR_STRING_DEFAULT_LENGTH 4
#endif

const char *TokenType_to_str(enum TokenType type) {
  switch (type) {
    case TOKENTYPE_UNKNOWN:
      return "TOKENTYPE_UNKNOWN";
    case TOKENTYPE_VAR:
      return "TOKENTYPE_VAR";
    case TOKENTYPE_COMBINATOR:
      return "TOKENTYPE_COMBINATOR";
    case TOKENTYPE_NEWLINE:
      return "TOKENTYPE_NEWLINE";
    case TOKENTYPE_WHITESPACE:
      return "TOKENTYPE_WHITESPACE";
    case TOKENTYPE_PAREN_OPEN:
      return "TOKENTYPE_PAREN_OPEN";
    case TOKENTYPE_PAREN_CLOSE:
      return "TOKENTYPE_PAREN_CLOSE";
    case TOKENTYPE_EOF:
      return "TOKENTYPE_EOF";
    }
  return nullptr;
}

const char *TokenFlags_to_str(enum TokenFlags flags) {
  switch (flags) {
    case TOKENFLAGS_NONE:
      return "None";
    case TOKENFLAGS_DYNAMICALLY_ALLOCATED:
      return "DA";
    case TOKENFLAGS_REPR_DYNAMICALLY_ALLOCATED:
      return "R:DA";
  }

  return nullptr;
}

char *Token_repr_str(enum TokenType type, union TokenRepr repr) {
  switch (type) {
    case TOKENTYPE_UNKNOWN:
      [[fallthrough]];
    case TOKENTYPE_VAR:
      return repr.str;
    case TOKENTYPE_COMBINATOR:
      switch (repr.combinator) {
        case COMBINATOR_I:
          return "I";
        case COMBINATOR_S:
          return "S";
        case COMBINATOR_K:
          return "K";
      }
    case TOKENTYPE_WHITESPACE:
      return " ";
    case TOKENTYPE_NEWLINE:
      return "\\n";
    case TOKENTYPE_PAREN_OPEN:
      return "(";
    case TOKENTYPE_PAREN_CLOSE:
      return ")";
    case TOKENTYPE_EOF:
      return "EOF";
    }

  return nullptr;
}

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

void Token_print(const Token *token) {
  printf("[Token] flags: %s, type: %s, repr: \"%s\"\n", TokenFlags_to_str(token->flags), TokenType_to_str(token->type), Token_repr_str(token->type, token->repr));
}

void TokenList_print(const TokenList *list) {
  for (size_t i = 0; i < list->occupied; i++) {
    Token_print(&list->tokens[i]);
  }
}

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
