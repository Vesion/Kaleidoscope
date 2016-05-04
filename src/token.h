#ifndef KAL_TOKEN_H
#define KAL_TOKEN_H

#include <string>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one of these for known things.
enum Token {
  tok_eof = -1,

  // keywords
  tok_def = -2,
  tok_extern = -3,

  // identifier and number literal
  tok_identifier = -4,
  tok_number = -5
};

extern std::string IdentifierStr; // Filled in if tok_identifier
extern double NumVal;             // Filled in if tok_number

// gettok - Return the next token from standard input.
int gettok();

#endif
