#include "parser.h"
#include "ast.h"
#include "spdlog/spdlog.h"

/*
expression     -> assignment ;
assignment     -> IDENTIFIER "=" assignment | logic_or ;
logic_or       -> logic_and ( "or" logic_and )* ;
logic_and      -> equality ( "and" equality )* ;
equality       -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison     -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           -> factor ( ( "-" | "+" ) factor )* ;
factor         -> unary ( ( "/" | "*" ) unary )* ;
unary          -> ( "!" | "-" ) unary | primary ;
primary        -> NUMBER | STRING | IDENTIFIER | "true" | "false" | "nil" | "("
expression
")" ;
*/

void Parser::init(const std::vector<Token> &tokens) {
  tokens_ = tokens;
  current_ = 0;
  return;
}

Expr *Parser::logic_or() {
  Expr *l_and = logic_and();
  if (l_and == NULL) {
    spdlog::error("[Parser::logic_or] Could not parse the expression");
    return NULL;
  }
  Token *op = new Token();
  if (peek(*op) && op->token_type_ == OR) {
    Token _;
    advance(_);
    Expr *r_and = logic_and();
    if (r_and == NULL) {
      spdlog::error("[Parser::logic_or] Could not parse the expression");
      return NULL;
    }
    Logical *l = new Logical();
    l->left = l_and;
    l->op = op;
    l->right = r_and;
    return l;
  } else {
    return l_and;
  }
}

Expr *Parser::logic_and() {
  Expr *l_eq = equality();
  if (l_eq == NULL) {
    spdlog::error("[Parser::logic_and] Could not parse the expression");
    return NULL;
  }
  Token *op = new Token();
  ;
  if (peek(*op) && op->token_type_ == AND) {
    Token _;
    advance(_);
    Expr *r_eq = equality();
    if (r_eq == NULL) {
      spdlog::error("[Parser::logic_and] Could not parse the expression");
      return NULL;
    }
    Logical *l = new Logical();
    l->left = l_eq;
    l->op = op;
    l->right = r_eq;
    return l;
  } else {
    return l_eq;
  }
}

Expr *Parser::assignment() {
  // try to match an equality or other subsumed
  // lower level expressions
  Expr *e = logic_or();
  if (e == NULL) {
    spdlog::error("Could not parse the expression");
    return NULL;
  }
  if (match({EQUAL})) {
    // if we still have a trailing equals then maybe lhs is an
    // assignable target. Check if the lhs evaluates to a variable
    // Right now, the only valid target is a simple variable expression, but
    // we’ll add fields later.
    Variable *v = dynamic_cast<Variable *>(e);
    if (v == nullptr) {
      // this is bad call a parser error
      spdlog::error("Could not parse the expression");
      return NULL;
    }
    // otherwise this means maybe there is a valid expression on the other side
    Expr *eval_expr = assignment();
    if (eval_expr == NULL) {
      spdlog::error("Could not parse the expression");
      return NULL;
    }
    Assign *a = new Assign();
    a->name = v->name;
    a->value = eval_expr;
    return a;
  } else {
    return e;
  }
}

Expr *Parser::expression() {
  spdlog::debug("Parsing expression");
  Expr *e = assignment();
  Token t;
  // if there is a token that is not a semicolon then there was a parser error!
  /* if (peek(t) && (t.token_type_ != SEMICOLON && t.token_type_ !=
   * END_OF_FILE)) { */
  /*   spdlog::error("Could not parse the expression. End of line does not have
   * " */
  /*                 "semicolon. The next token is {}", */
  /*                 token_type_to_str(t.token_type_).c_str()); */
  /*   return NULL; */
  /* } */
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
    Expr *expr = assignment();
    if (!match({RIGHT_PAREN})) {
      spdlog::error("Could not parse, missing right parenthesis");
      return NULL;
    }
    Token t;
    peek(t);
    spdlog::debug("Next token is {}", token_type_to_str(t.token_type_).c_str());
    return expr;
  } else if (match({IDENTIFIER})) {
    // if identifier then create a variable node
    Expr *expr = new Variable();
    static_cast<Variable *>(expr)->name = new Token();
    previous(*static_cast<Variable *>(expr)->name);
    spdlog::debug("Parsing identifier {}",
                  static_cast<Variable *>(expr)->name->literal_string.c_str());
    return expr;
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
    if (!expr) {
      return nullptr;
    }
    if (!match({SEMICOLON})) {
      report("Missing semicolon at the end of the statement", "", 0);
      return nullptr;
    }
    Print *p = new Print();
    p->expression = expr;
    return p;
    break;
  }
  case LEFT_BRACE: {
    // consume left brace
    Token _;
    advance(_);
    std::vector<Stmt *> stmts;
    if (!parse_block(stmts)) {
      return nullptr;
    }
    spdlog::debug("There are {} statements in the block", stmts.size());
    Block *b = new Block();
    b->statements = std::move(stmts);
    spdlog::debug("There are {} statements in the block", b->statements.size());
    return b;
    break;
  }
  case IF: {
    return parse_if();
  }
  case WHILE: {
    return parse_while();
  }
  case FOR: {
    return parse_for();
  }
  default: {
    return parse_expression_statement();
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
  // We seperate the declaration type statements from other statements because
  // we don't want to allow certain kinds of syntax like this:
  // if (monday) var beverage = "espresso"; Here the var declaration is
  // just inside an if block. That could be allowed but is pointless and
  // confusing to be allowed
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

bool Parser::parse_block(std::vector<Stmt *> &statements) {
  Token t;
  // we have tokens to parse and it's not a right paren
  spdlog::debug("Entered parsing block");
  while (!is_at_end() && peek(t) && t.token_type_ != RIGHT_BRACE) {
    spdlog::debug("Parsing statement for block");
    statements.push_back(parse_declaration());
  }
  if (!match({RIGHT_BRACE})) {
    report("Expected } after block", "", 0);
    return false;
  }
  return true;
}

std::vector<Stmt *> Parser::parse_stmts() {
  std::vector<Stmt *> statements;
  while (!is_at_end()) {
    auto expr = parse_declaration();
    if (!expr) {
      spdlog::error("Could not parse expression");
      return {};
    }
    statements.push_back(expr);
  }
  return statements;
}

Stmt *Parser::parse_if() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'if'.", "", 0);
    return nullptr;
  }
  Expr *e = expression();
  if (e == nullptr) {
    report("Expression inside if did not get evaluated", "", 0);
    return nullptr;
  }
  if (!match({RIGHT_PAREN})) {
    report("Expect ')' after 'if' condition", "", 0);
    return nullptr;
  }
  Stmt *thenBranch = parse_statement();
  if (thenBranch == nullptr) {
    return thenBranch;
  }
  Stmt *elseBranch = nullptr;
  if (match({ELSE})) {
    elseBranch = parse_statement();
    if (elseBranch == nullptr) {
      return elseBranch;
    }
  }
  If *i = new If();
  i->condition = e;
  i->thenBranch = thenBranch;
  i->elseBranch = elseBranch;
  return i;
}

Stmt *Parser::parse_while() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'while'.", "", 0);
    return nullptr;
  }
  Expr *e = expression();
  if (e == nullptr) {
    report("Expression inside while did not get evaluated", "", 0);
    return nullptr;
  }
  if (!match({RIGHT_PAREN})) {
    report("Expect ')' after 'while' condition", "", 0);
    return nullptr;
  }
  Stmt *whileBranch = parse_statement();
  if (whileBranch == nullptr) {
    return whileBranch;
  }
  While *w = new While();
  w->condition = e;
  w->body = whileBranch;
  return w;
}

Stmt *Parser::parse_for() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'for'.", "", 0);
    return nullptr;
  }
  Stmt *initializer = nullptr;
  if (match({SEMICOLON})) {
    initializer = nullptr;
  } else if (match({VAR})) {
    initializer = parse_var_declaration();
  } else {
    initializer = parse_expression_statement();
  }
  if (initializer == nullptr) {
    report("Could not parse initializer for for loop", "", 0);
    return nullptr;
  }
  Expr *condition = nullptr;
  if (!match({SEMICOLON})) {
    condition = expression();
    if (condition == nullptr) {
      report("Could not parse condition for for loop", "", 0);
      return nullptr;
    }
    if (!match({SEMICOLON})) {
      report("Expect ';' after loop condition.", "", 0);
      return nullptr;
    }
  }
  Expr *increment = nullptr;
  if (!match({RIGHT_PAREN})) {
    increment = expression();
    if (increment == nullptr) {
      report("Could not parse increment for for loop", "", 0);
      return nullptr;
    }
    if (!match({RIGHT_PAREN})) {
      report("Expect ')' after for clauses.", "", 0);
      return nullptr;
    }
  }
  Stmt *body = parse_statement();
  if (body == nullptr) {
    return body;
  }
  if (increment != nullptr) {
    std::vector<Stmt *> body_stmts;
    body_stmts.push_back(body);
    Expression *ex = new Expression();
    ex->expression = increment;
    body_stmts.push_back(ex);
    Block *b = new Block();
    b->statements = body_stmts;
    body = b;
  }
  if (condition == nullptr) {
    condition = new Literal();
    ((Literal *)condition)->value = new Token{FALSE, "false", 0, 0};
  }
  While *w = new While();
  w->condition = condition;
  w->body = body;
  if (initializer != nullptr) {
    std::vector<Stmt *> body_stmts;
    body_stmts.push_back(initializer);
    body_stmts.push_back(w);
    Block *b = new Block();
    b->statements = body_stmts;
    return b;
  }
  return w;
}

Stmt *Parser::parse_expression_statement() {
  // evaluate as an expression
  Expr *expr = expression();
  if (!expr) {
    return nullptr;
  }
  Expression *ex = new Expression();
  if (!match({SEMICOLON})) {
    report("Missing semicolon at the end of the statement", "", 0);
    return nullptr;
  }
  ex->expression = expr;
  return ex;
}
