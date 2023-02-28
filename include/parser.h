#pragma once

#include <vector>

#include "ast.h"
#include "token.h"

class Parser {
  std::vector<Token> tokens_;
  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> assignment();
  std::shared_ptr<Expr> equality();
  std::shared_ptr<Expr> comparison();
  std::shared_ptr<Expr> unary();
  std::shared_ptr<Expr> factor();
  std::shared_ptr<Expr> term();
  std::shared_ptr<Expr> primary();
  std::shared_ptr<Expr> logic_or();
  std::shared_ptr<Expr> logic_and();
  std::shared_ptr<Expr> call();
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
  std::shared_ptr<Expr> parse();
  std::vector<std::shared_ptr<Stmt>> parse_stmts();
  std::shared_ptr<Stmt> parse_declaration();
  std::shared_ptr<Stmt> parse_var_declaration();
  std::shared_ptr<Stmt> parse_statement();
  std::shared_ptr<Stmt> parse_if();
  std::shared_ptr<Stmt> parse_while();
  std::shared_ptr<Stmt> parse_expression_statement();
  std::shared_ptr<Stmt> parse_for();
  std::shared_ptr<Stmt> parse_function();
  std::shared_ptr<Stmt> parse_return();
  std::shared_ptr<Expr> finish_call(std::shared_ptr<Expr> expr);
  bool parse_block(std::vector<std::shared_ptr<Stmt>> &statements);
};
