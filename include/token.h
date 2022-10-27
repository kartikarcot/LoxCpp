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
  void *literal = nullptr;
  int line_no;
  void release_data() {
    if (literal != nullptr) {
      switch (token_type_) {
      case NUMBER:
        delete (float *)literal;
        break;
      case FALSE:
        delete (bool *)literal;
        break;
      case TRUE:
        delete (bool *)literal;
      default:
        break;
      }
    }
    literal = nullptr;
  }

  void copy_data(const Token &t) {
    this->literal_string = t.literal_string;
    this->token_type_ = t.token_type_;
    this->line_no = t.line_no;
    switch (t.token_type_) {
    case NUMBER:
      this->literal = new float(*(float *)t.literal);
      break;
    case FALSE:
      this->literal = new bool(*(bool *)t.literal);
      break;
    case TRUE:
      this->literal = new bool(*(bool *)t.literal);
      break;
    case STRING:
      this->literal = (void *)this->literal_string.c_str();
    default:
      break;
    }
  }

  ~Token() { release_data(); }

  // assignment operator
  Token &operator=(const Token &t) {
    this->release_data();
    this->copy_data(t);
    return *this;
  }

  Token(){};

  Token(TokenType t, std::string l, void *v, int line_no)
      : token_type_(t), literal_string(l), literal(v), line_no(line_no){};

  Token(const Token &t) { this->copy_data(t); }
};

std::string token_type_to_str(TokenType t);
