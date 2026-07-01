#include <stdio.h>
#include "token.h"


static inline char *Token_repr_str(enum TokenType type, union TokenRepr repr) {
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

static inline const char *TokenFlags_to_str(enum TokenFlags flags) {
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

static inline const char *TokenType_to_str(enum TokenType type) {
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

void Token_print(const Token *token) {
  printf("[Token] flags: %s, type: %s, repr: \"%s\"\n", TokenFlags_to_str(token->flags), TokenType_to_str(token->type), Token_repr_str(token->type, token->repr));
}
