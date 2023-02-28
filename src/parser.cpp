#include "parser.h"
#include "ast.h"
#include "logger.h"
#include "printer.h"
#include "token.h"
#include <memory>

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

int Parser::get_current_line() {
  if (is_at_end() || current_ > tokens_.size()) {
    return tokens_[current_ - 1].line_no;
  }
  if (current_ < 0) {
    report("Report this error to askarthikkumar@gmail.com", "", -1);
    return -1;
  }
  return tokens_[current_].line_no;
}

std::shared_ptr<Expr> Parser::logic_or() {
  auto l_and = logic_and();
  if (l_and == nullptr) {
    return nullptr;
  }
  auto op = std::make_shared<Token>();
  if (peek(*op) && op->token_type_ == OR) {
    Token _;
    advance(_);
    auto r_and = logic_and();
    if (r_and == nullptr) {
      return nullptr;
    }
    auto l = std::make_shared<Logical>();
    l->left = l_and;
    l->op = op;
    l->right = r_and;
    l->line_no = op->line_no;
    return l;
  } else {
    return l_and;
  }
}

std::shared_ptr<Expr> Parser::logic_and() {
  std::shared_ptr<Expr> l_eq = equality();
  if (l_eq == nullptr) {
    return nullptr;
  }
  auto op = std::make_shared<Token>();
  ;
  if (peek(*op) && op->token_type_ == AND) {
    Token _;
    advance(_);
    std::shared_ptr<Expr> r_eq = equality();
    if (r_eq == nullptr) {
      return nullptr;
    }
    auto l = std::make_shared<Logical>();
    l->left = l_eq;
    l->op = op;
    l->right = r_eq;
    l->line_no = op->line_no;
    return l;
  } else {
    return l_eq;
  }
}

std::shared_ptr<Expr> Parser::assignment() {
  // try to match an equality or other subsumed
  // lower level expressions
  std::shared_ptr<Expr> e = logic_or();
  if (e == nullptr) {
    return nullptr;
  }
  if (match({EQUAL})) {
    // if we still have a trailing equals then maybe lhs is an
    // assignable target. Check if the lhs evaluates to a variable
    // Right now, the only valid target is a simple variable expression, but
    // we’ll add fields later.
    std::shared_ptr<Variable> v = dynamic_pointer_cast<Variable>(e);
    if (v == nullptr) {
      // this is bad call a parser error
      report("Invalid assignment target.", "", e->line_no);
      return nullptr;
    }
    // otherwise this means maybe there is a valid expression on the other side
    std::shared_ptr<Expr> eval_expr = assignment();
    if (eval_expr == nullptr) {
      return nullptr;
    }
    auto a = std::make_shared<Assign>();
    a->name = v->name;
    a->value = eval_expr;
    a->line_no = e->line_no;
    return a;
  } else {
    return e;
  }
}

std::shared_ptr<Expr> Parser::expression() {
  std::shared_ptr<Expr> e = assignment();
  Token t;
  return e;
}

std::shared_ptr<Expr> Parser::equality() {
  auto expr = comparison();
  if (expr == nullptr) {
    return nullptr;
  }
  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op;
    if (!previous(op)) {
      report("Issue parsing equality", "", tokens_[current_].line_no);
      return nullptr;
    }
    std::shared_ptr<Expr> right = comparison();
    if (right == nullptr) {
      return nullptr;
    }
    std::shared_ptr<Expr> new_expr = std::make_shared<Binary>();
    static_pointer_cast<Binary>(new_expr)->left = expr;
    static_pointer_cast<Binary>(new_expr)->op = std::make_shared<Token>(op);
    static_pointer_cast<Binary>(new_expr)->right = right;
    expr = new_expr;
    expr->line_no = op.line_no;
  }
  return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
  auto expr = term();
  if (expr == nullptr) {
    return nullptr;
  }
  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op;
    if (!previous(op)) {
      report("Issue parsing comparison", "", tokens_[current_].line_no);
      return nullptr;
    }
    auto right = term();
    if (right == nullptr) {
      return nullptr;
    }
    std::shared_ptr<Expr> new_expr = std::make_shared<Binary>();
    static_pointer_cast<Binary>(new_expr)->left = expr;
    static_pointer_cast<Binary>(new_expr)->op = std::make_shared<Token>(op);
    static_pointer_cast<Binary>(new_expr)->right = right;
    expr = new_expr;
    expr->line_no = op.line_no;
  }

  return expr;
}

std::shared_ptr<Expr> Parser::term() {
  auto expr = factor();
  if (expr == nullptr) {
    return nullptr;
  }
  while (match({PLUS, MINUS})) {
    Token op;
    if (!previous(op)) {
      report("Issue parsing term", "", tokens_[current_].line_no);
      return nullptr;
    }
    auto right = factor();
    if (right == nullptr) {
      return nullptr;
    }
    auto new_expr = std::make_shared<Binary>();
    static_pointer_cast<Binary>(new_expr)->left = expr;
    static_pointer_cast<Binary>(new_expr)->op = std::make_shared<Token>(op);
    static_pointer_cast<Binary>(new_expr)->right = right;
    expr = new_expr;
    expr->line_no = op.line_no;
  }
  return expr;
}

std::shared_ptr<Expr> Parser::factor() {
  auto expr = unary();
  if (expr == nullptr) {
    return nullptr;
  }
  while (match({SLASH, STAR})) {
    Token op;
    if (!previous(op)) {
      report("Issue parsing factor", "", tokens_[current_].line_no);
      return nullptr;
    }
    auto right = unary();
    if (right == nullptr) {
      return nullptr;
    }
    auto new_expr = std::make_shared<Binary>();
    static_pointer_cast<Binary>(new_expr)->left = expr;
    static_pointer_cast<Binary>(new_expr)->op = std::make_shared<Token>(op);
    static_pointer_cast<Binary>(new_expr)->right = right;
    expr = new_expr;
    expr->line_no = op.line_no;
  }
  return expr;
}

std::shared_ptr<Expr> Parser::unary() {
  if (match({BANG, MINUS})) {
    Token op;
    if (!previous(op)) {
      report("Issue parsing code", "", tokens_[current_].line_no);
      return nullptr;
    }
    auto op_expr = unary();
    auto expr = std::make_shared<Unary>();
    static_pointer_cast<Unary>(expr)->op = std::make_shared<Token>(op);
    static_pointer_cast<Unary>(expr)->right = op_expr;
    expr->line_no = op.line_no;
    return expr;
  }
  return call();
}

std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> expr) {
  // we have a left paren
  // we need to parse the arguments if any exist
  // and then the right paren
  std::vector<std::shared_ptr<Expr>> args;
  Token t;
  if (!peek(t)) {
    report("Issue parsing call", "", get_current_line());
    return nullptr;
  }
  if (t.token_type_ != RIGHT_PAREN) {
    do {
      auto arg = assignment();
      if (arg == nullptr) {
        return nullptr;
      }
      args.push_back(arg);
      if (args.size() > kMaxArgs) {
        report("Too many arguments. Max number of arguments supported is " +
                   std::to_string(kMaxArgs),
               "", get_current_line());
        return nullptr;
      }
    } while (match({COMMA}));
  }
  // match the right paren
  if (!advance(t) || t.token_type_ != RIGHT_PAREN) {
    report("Expected )", "", get_current_line());
    return nullptr;
  }
  // construct the call object
  auto call = std::make_shared<Call>();
  call->callee = expr;
  call->arguments = args;
  call->paren = std::make_shared<Token>(t);
  return call;
}

std::shared_ptr<Expr> Parser::call() {
  std::shared_ptr<Expr> expr = primary();
  if (expr == nullptr) {
    return nullptr;
  }
  // to allow for calls like fn(a)(b). Aka currying
  while (true) {
    if (match({LEFT_PAREN})) {
      expr = finish_call(expr);
    } else {
      break;
    }
  }
  return expr;
}

std::shared_ptr<Expr> Parser::primary() {
  if (match({LEFT_PAREN})) {
    // if a grouped then start from the top
    std::shared_ptr<Expr> expr = assignment();
    if (!match({RIGHT_PAREN})) {
      report("Expected )", "", get_current_line());
      return nullptr;
    }
    Token t;
    peek(t);
    expr->line_no = get_current_line();
    return expr;
  } else if (match({IDENTIFIER})) {
    // if identifier then create a variable node
    auto expr = std::make_shared<Variable>();
    static_pointer_cast<Variable>(expr)->name = std::make_shared<Token>();
    previous(*static_pointer_cast<Variable>(expr)->name);
    expr->line_no = get_current_line();
    return expr;
  } else {
    // otherwise it has to be a literal or some parse error
    Token t;
    if (!peek(t)) {
      if (is_at_end()) {
        report("Unexpected end of file", "", get_current_line());
      } else {
        report("Report this error to askarthikkumar@gmail.com", "", -1);
      }
      return nullptr;
    }
    auto expr = std::make_shared<Literal>();
    static_pointer_cast<Literal>(expr)->value = std::make_shared<Token>(t);
    expr->line_no = get_current_line();
    advance(t);
    return expr;
  }
  return nullptr;
}

Token Parser::consume(TokenType type, std::string message) {
  Token t;
  if (!peek(t)) {
    report("Unexpected end of file", "", get_current_line());
    return t;
  }
  if (t.token_type_ == type) {
    advance(t);
    return t;
  }
  report(message, "", get_current_line());
  return t;
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

std::shared_ptr<Expr> Parser::parse() { return expression(); }

std::shared_ptr<Stmt> Parser::parse_statement() {
  // consume statement
  Token t;
  peek(t);
  switch (t.token_type_) {
  case PRINT: {
    match({PRINT});
    // match a left paren
    if (!match({LEFT_PAREN})) {
      report("Expected (", "", get_current_line());
      return nullptr;
    }
    std::vector<std::shared_ptr<Expr>> expressions;
    // read a list of comma seperated expressions
    while (!match({RIGHT_PAREN})) {
      // read a single expression
      auto expr = expression();
      if (!expr) {
        return nullptr;
      }
      expressions.push_back(expr);
      // if we don't find a comma and peek doesn't return a right paren
      // then we have a syntax error
      if (!match({COMMA})) {
        if (match({RIGHT_PAREN})) {
          break;
        }
        report("Expected , or )", "", get_current_line());
        return nullptr;
      }
    }
    if (!match({SEMICOLON})) {
      report("Missing semicolon at the end of the statement", "",
             get_current_line());
      return nullptr;
    }
    auto p = std::make_shared<Print>();
    p->expressions = expressions;
    p->line_no = t.line_no;
    return p;
    break;
  }
  case LEFT_BRACE: {
    // consume left brace
    Token _;
    advance(_);
    std::vector<std::shared_ptr<Stmt>> stmts;
    if (!parse_block(stmts)) {
      return nullptr;
    }
    auto b = std::make_shared<Block>();
    b->statements = std::move(stmts);
    b->line_no = t.line_no;
    return b;
    break;
  }
  case IF: {
    auto if_st = parse_if();
    if_st->line_no = t.line_no;
    return if_st;
  }
  case WHILE: {
    auto while_st = parse_while();
    while_st->line_no = t.line_no;
    return while_st;
  }
  case FOR: {
    auto for_st = parse_for();
    for_st->line_no = t.line_no;
    return for_st;
  }
  case RETURN: {
    match({RETURN});
    auto return_st = parse_return();
    return_st->line_no = t.line_no;
    return return_st;
  }
  default: {
    auto expr_st = parse_expression_statement();
    if (expr_st) {
      expr_st->line_no = t.line_no;
    }
    return expr_st;
  }
  }
}

std::shared_ptr<Stmt> Parser::parse_var_declaration() {
  auto t = std::make_shared<Token>();
  if (!advance(*t) || t->token_type_ != IDENTIFIER) {
    report("Missing/Invalid identifier in variable declaration statement", "",
           get_current_line());
  }
  if (match({EQUAL})) {
    auto ex = expression();
    if (ex) {
      auto var = std::make_shared<Var>();
      var->name = t;
      var->initializer = ex;
      if (match({SEMICOLON})) {
        var->line_no = t->line_no;
        return var;
      } else {
        report("Missing ; in variable declaration statement", "",
               get_current_line());
      }
    } else {
      report("Could not parse the expression assigned to the variable", "",
             get_current_line());
    }
  } else {
    report("Missing = in variable declaration statement", "",
           get_current_line());
  }
  return nullptr;
}

std::shared_ptr<Stmt> Parser::parse_function() {
  Token t;
  if (peek(t) && t.token_type_ == IDENTIFIER) {
    advance(t);
  } else {
    report("Missing/Invalid identifier in function declaration statement", "",
           get_current_line());
    return nullptr;
  }
  auto name = t;
  if (!match({LEFT_PAREN})) {
    report("Missing ( in function declaration statement", "",
           get_current_line());
    return nullptr;
  }
  std::vector<std::shared_ptr<Token>> params;
  while (!match({RIGHT_PAREN})) {
    Token t;
    if (!peek(t) || t.token_type_ != IDENTIFIER) {
      report("Missing/Invalid identifier in function declaration statement", "",
             get_current_line());
      return nullptr;
    }
    advance(t);
    params.push_back(std::make_shared<Token>(t));
    if (params.size() > kMaxArgs) {
      report("Too many arguments in function declaration statement", "",
             get_current_line());
      return nullptr;
    }
    if (!match({COMMA})) {
      if (!match({RIGHT_PAREN})) {
        report("Missing ) or , in function declaration statement", "",
               get_current_line());
        return nullptr;
        break;
      } else {
        // The we have matched a right paren, we must break
        // to read the body
        break;
      }
    }
  }
  // we have matched the fun <name> ( <params> ) part of the signature
  // now we must parse the body
  if (peek(t) && t.token_type_ == LEFT_BRACE) {
    advance(t);
  } else {
    report("Missing { in function declaration statement", "",
           get_current_line());
    return nullptr;
  }
  std::vector<std::shared_ptr<Stmt>> body;
  if (!parse_block(body)) {
    // cast to Block and iterate over the statements
    return nullptr;
  }

  auto f = std::make_shared<Function>();
  f->name = std::make_shared<Token>(name);
  f->params = params;
  f->body = body;
  return f;
}

std::shared_ptr<Stmt> Parser::parse_declaration() {
  // We seperate the declaration type statements from other statements because
  // we don't want to allow certain kinds of syntax like this:
  // if (monday) var beverage = "espresso"; Here the var declaration is
  // just inside an if block. That could be allowed but is pointless and
  // confusing to be allowed
  std::shared_ptr<Stmt> s = nullptr;
  // if we match a var start point then we are a variable declaration
  if (match({VAR})) {
    s = parse_var_declaration();
  } else if (match({FUN})) {
    s = parse_function();
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

bool Parser::parse_block(std::vector<std::shared_ptr<Stmt>> &statements) {
  Token t;
  // we have tokens to parse and it's not a right paren
  while (!is_at_end() && peek(t) && t.token_type_ != RIGHT_BRACE) {
    auto expr = parse_declaration();
    if (!expr) {
      return false;
    }
    statements.push_back(expr);
  }
  if (!match({RIGHT_BRACE})) {
    report("Expected } after block", "", get_current_line());
    return false;
  }
  return true;
}

std::vector<std::shared_ptr<Stmt>> Parser::parse_stmts() {
  std::vector<std::shared_ptr<Stmt>> statements;
  while (!is_at_end()) {
    auto expr = parse_declaration();
    if (!expr) {
      return {};
    }
    expr->line_no = tokens_[current_].line_no;
    statements.push_back(expr);
  }
  return statements;
}

std::shared_ptr<Stmt> Parser::parse_if() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'if'.", "", t.line_no);
    return nullptr;
  }
  std::shared_ptr<Expr> e = expression();
  if (e == nullptr) {
    return nullptr;
  }
  if (!match({RIGHT_PAREN})) {
    report("Expect ')' after 'if' condition", "", get_current_line());
    return nullptr;
  }
  std::shared_ptr<Stmt> thenBranch = parse_statement();
  if (thenBranch == nullptr) {
    return thenBranch;
  }
  std::shared_ptr<Stmt> elseBranch = nullptr;
  if (match({ELSE})) {
    elseBranch = parse_statement();
    if (elseBranch == nullptr) {
      return elseBranch;
    }
  }
  auto i = std::make_shared<If>();
  i->condition = e;
  i->thenBranch = thenBranch;
  i->elseBranch = elseBranch;
  return i;
}

std::shared_ptr<Stmt> Parser::parse_while() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'while'.", "", get_current_line());
    return nullptr;
  }
  std::shared_ptr<Expr> e = expression();
  if (e == nullptr) {
    report("Expression inside while did not get evaluated", "",
           get_current_line());
    return nullptr;
  }
  if (!match({RIGHT_PAREN})) {
    report("Expect ')' after 'while' condition", "", get_current_line());
    return nullptr;
  }
  std::shared_ptr<Stmt> whileBranch = parse_statement();
  if (whileBranch == nullptr) {
    return whileBranch;
  }
  auto w = std::make_shared<While>();
  w->condition = e;
  w->body = whileBranch;
  return w;
}

std::shared_ptr<Stmt> Parser::parse_for() {
  Token t;
  advance(t);
  if (!match({LEFT_PAREN})) {
    report("Expect '(' after 'for'.", "", get_current_line());
    return nullptr;
  }
  std::shared_ptr<Stmt> initializer = nullptr;
  if (match({SEMICOLON})) {
    initializer = nullptr;
  } else if (match({VAR})) {
    initializer = parse_var_declaration();
  } else {
    initializer = parse_expression_statement();
  }
  if (initializer == nullptr) {
    report("Could not parse initializer for for loop", "", get_current_line());
    return nullptr;
  }
  std::shared_ptr<Expr> condition = nullptr;
  if (!match({SEMICOLON})) {
    condition = expression();
    if (condition == nullptr) {
      report("Could not parse condition for for loop", "", get_current_line());
      return nullptr;
    }
    if (!match({SEMICOLON})) {
      report("Expect ';' after loop condition.", "", get_current_line());
      return nullptr;
    }
  }
  std::shared_ptr<Expr> increment = nullptr;
  if (!match({RIGHT_PAREN})) {
    increment = expression();
    if (increment == nullptr) {
      report("Could not parse increment for for loop", "", get_current_line());
      return nullptr;
    }
    if (!match({RIGHT_PAREN})) {
      report("Expect ')' after for clauses.", "", get_current_line());
      return nullptr;
    }
  }
  std::shared_ptr<Stmt> body = parse_statement();
  if (body == nullptr) {
    return body;
  }
  if (increment != nullptr) {
    std::vector<std::shared_ptr<Stmt>> body_stmts;
    body_stmts.push_back(body);
    auto ex = std::make_shared<Expression>();
    ex->expression = increment;
    body_stmts.push_back(ex);
    auto b = std::make_shared<Block>();
    b->statements = body_stmts;
    body = b;
  }
  if (condition == nullptr) {
    condition = std::make_shared<Literal>();
    dynamic_pointer_cast<Literal>(condition)->value =
        std::make_shared<Token>(Token{FALSE, "false", 0, 0});
  }
  auto w = std::make_shared<While>();
  w->condition = condition;
  w->body = body;
  if (initializer != nullptr) {
    std::vector<std::shared_ptr<Stmt>> body_stmts;
    body_stmts.push_back(initializer);
    body_stmts.push_back(w);
    auto b = std::make_shared<Block>();
    b->statements = body_stmts;
    return b;
  }
  return w;
}

std::shared_ptr<Stmt> Parser::parse_expression_statement() {
  // evaluate as an expression
  std::shared_ptr<Expr> expr = expression();
  if (!expr) {
    return nullptr;
  }
  auto ex = std::make_shared<Expression>();
  if (!match({SEMICOLON})) {
    report("Missing semicolon at the end of the statement", "",
           get_current_line());
    return nullptr;
  }
  ex->expression = expr;
  return ex;
}

std::shared_ptr<Stmt> Parser::parse_return() {
  Token ret;
  previous(ret);
  std::shared_ptr<Expr> expr = nullptr;
  if (!match({SEMICOLON})) {
    expr = expression();
    PrettyPrinter p;
    CLog::FLog(LogLevel::DEBUG, LogCategory::PARSER, "%s",
               p.paranthesize(expr.get()).c_str());
  }
  if (!match({SEMICOLON})) {
    report("Expect ';' after return value.", "", get_current_line());
    return nullptr;
  }
  auto r = std::make_shared<Return>();
  r->keyword = std::make_shared<Token>(ret);
  r->value = expr;
  return r;
}
