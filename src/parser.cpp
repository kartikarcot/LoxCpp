#include "parser.h"
#include "ast.h"
#include "spdlog/spdlog.h"

/*
expression     -> equality ;
equality       -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison     -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           -> factor ( ( "-" | "+" ) factor )* ;
factor         -> unary ( ( "/" | "*" ) unary )* ;
unary          -> ( "!" | "-" ) unary | primary ;
primary        -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression
")" ;
*/

void Parser::init(const std::vector<Token> &tokens) {
  tokens_ = tokens;
  current_ = 0;
  return;
}

Expr *Parser::expression() {
  spdlog::info("Parsing expression");
  return equality();
}

Expr *Parser::equality() {
  spdlog::info("Parsing equality");
  Expr *expr;
  expr = comparison();
  if (expr == NULL) {
    return NULL;
  }
  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op;
    if (!previous(op)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *right = comparison();
    if (right == NULL) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *new_expr = new Binary();
    static_cast<Binary *>(new_expr)->left = expr;
    static_cast<Binary *>(new_expr)->op = new Token(op);
    static_cast<Binary *>(new_expr)->right = right;
    expr = new_expr;
  }
  return expr;
}

Expr *Parser::comparison() {
  spdlog::info("Parsing comparison");
  Expr *expr;
  expr = term();
  if (expr == NULL) {
    return NULL;
  }
  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op;
    if (!previous(op)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *right = term();
    if (right == NULL) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *new_expr = new Binary();
    static_cast<Binary *>(new_expr)->left = expr;
    static_cast<Binary *>(new_expr)->op = new Token(op);
    static_cast<Binary *>(new_expr)->right = right;
    expr = new_expr;
  }

  return expr;
}

Expr *Parser::term() {
  spdlog::info("Parsing term");
  Expr *expr;
  expr = factor();
  if (expr == NULL) {
    return NULL;
  }
  while (match({PLUS, MINUS})) {
    Token op;
    if (!previous(op)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *right = factor();
    if (right == NULL) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *new_expr = new Binary();
    static_cast<Binary *>(new_expr)->left = expr;
    static_cast<Binary *>(new_expr)->op = new Token(op);
    static_cast<Binary *>(new_expr)->right = right;
    expr = new_expr;
  }

  return expr;
}

Expr *Parser::factor() {
  spdlog::info("Parsing factor");
  Expr *expr;
  expr = unary();
  if (expr == NULL) {
    return NULL;
  }
  while (match({SLASH, STAR})) {
    Token op;
    if (!previous(op)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *right = unary();
    if (right == NULL) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *new_expr = new Binary();
    static_cast<Binary *>(new_expr)->left = expr;
    static_cast<Binary *>(new_expr)->op = new Token(op);
    static_cast<Binary *>(new_expr)->right = right;
    expr = new_expr;
  }

  return expr;
}

Expr *Parser::unary() {
  spdlog::info("Parsing unary");
  if (match({BANG, MINUS})) {
    Token op;
    if (!previous(op)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *op_expr = unary();
    Expr *expr = new Unary();
    static_cast<Unary *>(expr)->op = new Token(op);
    static_cast<Unary *>(expr)->right = op_expr;
    return expr;
  }
  return primary();
}

Expr *Parser::primary() {
  spdlog::info("Parsing primary");
  if (match({LEFT_PAREN})) {
    Expr *expr = expression();
    if (!match({RIGHT_PAREN})) {
      spdlog::error("Could not parse");
      return NULL;
    }
	return expr;
  } else {
    Token t;
    if (!peek(t)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *expr = new Literal();
    static_cast<Literal *>(expr)->value = new Token(t);
	advance(t);
    return expr;
  }
  return NULL;
}

bool Parser::peek(Token &t) {
  if (is_at_end())
    return false;
  if (current_ < 0 || current_ >= tokens_.size())
    return false;
  t = tokens_[current_];
  return true;
}

bool Parser::is_at_end() {
  if (tokens_[current_].token_type_ == END_OF_FILE)
    return true;
  return false;
}

bool Parser::match(std::vector<TokenType> options) {
  Token t;
  if (!peek(t))
    return false;
  for (const auto &op : options) {
    if (t.token_type_ == op) {
      Token _;
      advance(_);
      return true;
    }
  }
  return false;
}

bool Parser::advance(Token &t) {
  if (is_at_end())
    return false;
  current_++;
  return previous(t);
}

bool Parser::previous(Token &t) {
  if (current_ == 0)
    return false;
  t = tokens_[current_ - 1];
  return true;
}

Expr *Parser::parse() { return expression(); }
