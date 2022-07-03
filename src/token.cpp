#include "token.h"

#define CASE_FN(x)                                                             \
  case x:                                                                      \
    return #x;

std::string token_type_to_str(TokenType t) {
  switch (t) {
    CASE_FN(LEFT_PAREN)  // [
    CASE_FN(RIGHT_PAREN) // ]
    CASE_FN(LEFT_BRACE)  // {
    CASE_FN(RIGHT_BRACE) // }
    CASE_FN(COMMA)       // ,
    CASE_FN(DOT)         // .
    CASE_FN(MINUS)       // -
    CASE_FN(PLUS)        // +
    CASE_FN(SEMICOLON)   //;
    CASE_FN(SLASH)       // /
    CASE_FN(STAR)        // *

    // logical and other operators
    CASE_FN(BANG)          // !
    CASE_FN(BANG_EQUAL)    // !=
    CASE_FN(EQUAL)         // =
    CASE_FN(EQUAL_EQUAL)   // ==
    CASE_FN(GREATER)       // >
    CASE_FN(GREATER_EQUAL) // >=
    CASE_FN(LESS)          // <
    CASE_FN(LESS_EQUAL)    // <=
    CASE_FN(SLASH_SLASH)   // //

    // literals
    CASE_FN(IDENTIFIER) // [_a-zA-Z][a-zA-Z0-9]*
    CASE_FN(STRING)     // "[.*]"
    CASE_FN(NUMBER)     // [0-9]+(\.[0-9]+)

    // reserved keywords
    CASE_FN(AND)    // and
    CASE_FN(CLASS)  // class
    CASE_FN(ELSE)   // else
    CASE_FN(FALSE)  // false
    CASE_FN(FUN)    // fun
    CASE_FN(FOR)    // for
    CASE_FN(IF)     // if
    CASE_FN(NIL)    // nil
    CASE_FN(OR)     // or
    CASE_FN(PRINT)  // print
    CASE_FN(RETURN) // return
    CASE_FN(SUPER)  // super
    CASE_FN(THIS)   // this
    CASE_FN(TRUE)   // true
    CASE_FN(VAR)    // var
    CASE_FN(WHILE)  // while

    CASE_FN(END_OF_FILE)
  }
  return "";
}
