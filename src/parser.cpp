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
  spdlog::debug("Parsing expression");
  Expr *e = equality();
  Token t;
  // if there is a token that is not a semicolon then there was a parser error!
  if (peek(t) && (t.token_type_ != SEMICOLON && t.token_type_ != END_OF_FILE)) {
    spdlog::error("Could not parse the expression");
    return NULL;
  }
  return e;
}

Expr *Parser::equality() {
  spdlog::debug("Parsing equality");
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
  spdlog::debug("Parsing comparison");
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
  spdlog::debug("Parsing term");
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
  spdlog::debug("Parsing factor");
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
  spdlog::debug("Parsing unary");
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
  spdlog::debug("Parsing primary");
  if (match({LEFT_PAREN})) {
    // if a grouped then start from the top
    Expr *expr = expression();
    if (!match({RIGHT_PAREN})) {
      spdlog::error("Could not parse");
      return NULL;
    }
    return expr;
  } else if (match({IDENTIFIER})) {
    // if identifier then create a variable node
    Variable *var = new Variable();
    var->name = new Token();
    previous(*var->name);
    spdlog::debug("Parsing identifier {}", var->name->literal_string.c_str());
    return var;
  } else {
    // otherwise it has to be a literal or some parse error
    Token t;
    if (!peek(t)) {
      spdlog::error("Could not parse");
      return NULL;
    }
    Expr *expr = new Literal();
    static_cast<Literal *>(expr)->value = new Token(t);
    spdlog::debug("Parsing literal {}", t.literal_string.c_str());
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

// This function advances our token iterator to the next statement
// We will use this when we fail to parse a statement.
void Parser::synchronize() {
  Token t;
  advance(t);
  while (!is_at_end()) {
    if (previous(t) && t.token_type_ == SEMICOLON)
      return;
    peek(t);
    switch (t.token_type_) {
    case CLASS:
    case FUN:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    default:
      advance(t);
      break;
    }
  }
}

Expr *Parser::parse() { return expression(); }

Stmt *Parser::parse_statement() {
  // consume statement
  Token t;
  peek(t);
  switch (t.token_type_) {
  case PRINT: {
    match({PRINT});
    Expr *expr = expression();
    if (!match({SEMICOLON})) {
      report("Missing semicolon at the end of the statement", "", 0);
      return {};
    }
    Print *p = new Print();
    p->expression = expr;
    return p;
    break;
  }
  default: {
    // evaluate as an expression
    Expr *expr = expression();
    Expression *ex = new Expression();
    if (!match({SEMICOLON})) {
      report("Missing semicolon at the end of the statement", "", 0);
      return {};
    }
    ex->expression = expr;
    return ex;
    break;
  }
  }
}

Stmt *Parser::parse_var_declaration() {
  Token *t = new Token();
  if (!advance(*t) || t->token_type_ != IDENTIFIER) {
    report("Missing/Invalid identifier in variable declaration statement", "",
           0);
  }
  if (match({EQUAL})) {
    Expr *ex = expression();
    if (ex) {
      Var *var = new Var();
      var->name = t;
      var->initializer = ex;
      if (match({SEMICOLON})) {
        return var;
      } else {
        report("Missing ; in variable declaration statement", "", 0);
      }
    } else {
      report("Could not parse the expression assigned to the variable", "", 0);
    }
  } else {
    report("Missing = in variable declaration statement", "", 0);
  }
  // if we reached here we should delete heap variable that was not used
  delete t;
  return nullptr;
}

Stmt *Parser::parse_declaration() {
  Stmt *s = nullptr;
  // if we match a var start point then we are a variable declaration
  if (match({VAR})) {
    s = parse_var_declaration();

  } else {
    // else parse it as a statement
    s = parse_statement();
  }
  // if parse failed then synchronize
  if (s == nullptr) {
    synchronize();
  }
  // return a parse-tree expression/NULL
  return s;
}

std::vector<Stmt *> Parser::parse_stmts() {
  std::vector<Stmt *> statements;
  while (!is_at_end()) {
    statements.push_back(parse_declaration());
  }
  return statements;
}
