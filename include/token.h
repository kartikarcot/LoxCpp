#pragma once
#include <regex>
#include <string>
#include <unordered_map>

enum TokenType {
  // punctuation
  LEFT_PAREN,  // (
  RIGHT_PAREN, // )
  LEFT_BRACE,  // {
  RIGHT_BRACE, // }
  COMMA,       // ,
  DOT,         // .
  MINUS,       // -
  PLUS,        // +
  SEMICOLON,   //;
  SLASH,       // /
  STAR,        // *

  // logical and other operators
  BANG,          // !
  BANG_EQUAL,    // !=
  EQUAL,         // =
  EQUAL_EQUAL,   // ==
  GREATER,       // >
  GREATER_EQUAL, // >=
  LESS,          // <
  LESS_EQUAL,    // <=
  SLASH_SLASH,   // //

  // literals
  IDENTIFIER, // [_a-zA-Z][a-zA-Z0-9]*
  STRING,     // "[.*]"
  NUMBER,     // [0-9]+(\.[0-9]+)

  // reserved keywords
  AND,    // and
  CLASS,  // class
  ELSE,   // else
  FALSE,  // false
  FUN,    // fun
  FOR,    // for
  IF,     // if
  NIL,    // nil
  OR,     // or
  PRINT,  // print
  RETURN, // return
  SUPER,  // super
  THIS,   // this
  TRUE,   // true
  VAR,    // var
  WHILE,  // while

  END_OF_FILE
};

struct Token {
  TokenType token_type_;
  std::string literal_string;
  void *literal;
  int line_no;
};


std::string token_type_to_str(TokenType t);
