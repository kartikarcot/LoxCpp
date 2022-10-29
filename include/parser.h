#pragma once

#include <vector>

#include "ast.h"
#include "token.h"

class Parser {
  std::vector<Token> tokens_;
  Expr *expression();
  Expr *equality();
  Expr *comparison();
  Expr *unary();
  Expr *factor();
  Expr *term();
  Expr *primary();
  bool match(std::vector<TokenType> options);
  bool peek(Token &t);
  bool advance(Token &t);
  bool previous(Token &t);
  bool is_at_end();
  int current_ = -1;
  void synchronize();

public:
  void init(const std::vector<Token> &tokens);
  Expr *parse();
  std::vector<Stmt *> parse_stmts();
  Stmt *parse_declaration();
  Stmt *parse_var_declaration();
  Stmt *parse_statement();
};
