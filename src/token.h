#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>

enum TokenType: uint32_t {
  TOKENTYPE_UNKNOWN = 0,
  TOKENTYPE_EOF = 0x1,

  TOKENTYPE_VAR = 0x2,
  TOKENTYPE_COMBINATOR = 0x3,

  TOKENTYPE_WHITESPACE = 0x4,
  TOKENTYPE_NEWLINE = 0x5,
  
  TOKENTYPE_PAREN_OPEN = 0x10,
  TOKENTYPE_PAREN_CLOSE = 0x11,

};

enum TokenFlags: uint32_t {
  TOKENFLAGS_NONE = 0,
  TOKENFLAGS_DYNAMICALLY_ALLOCATED = 0x1,
  TOKENFLAGS_REPR_DYNAMICALLY_ALLOCATED = 0x2
};


enum CombinatorTypes: uint8_t {
  COMBINATOR_I = 0,
  COMBINATOR_S,
  COMBINATOR_K
};

union TokenRepr {
  enum CombinatorTypes combinator;
  char *str;
};

typedef struct Token {
  enum TokenFlags flags;
  enum TokenType type;
  union TokenRepr repr;
} Token;

void Token_print(const Token *token);
#endif
