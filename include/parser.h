#pragma once

#include <vector>

#include "ast.h"
#include "token.h"

class Parser {
  std::vector<Token> tokens_;
  Expr *expression();
  Expr *assignment();
  Expr *equality();
  Expr *comparison();
  Expr *unary();
  Expr *factor();
  Expr *term();
  Expr *primary();
  Expr *logic_or();
  Expr *logic_and();
  Expr *call();
  bool match(std::vector<TokenType> options);
  bool peek(Token &t);
  bool advance(Token &t);
  bool previous(Token &t);
  Token consume(TokenType type, std::string message);
  bool is_at_end();
  int current_ = -1;
  const int kMaxArgs = 255;
  void synchronize();
  int get_current_line();

public:
  void init(const std::vector<Token> &tokens);
  Expr *parse();
  std::vector<Stmt *> parse_stmts();
  Stmt *parse_declaration();
  Stmt *parse_var_declaration();
  Stmt *parse_statement();
  Stmt *parse_if();
  Stmt *parse_while();
  Stmt *parse_expression_statement();
  Stmt *parse_for();
  Stmt *parse_function();
  Expr *finish_call(Expr *expr);
  bool parse_block(std::vector<Stmt *> &statements);
};
