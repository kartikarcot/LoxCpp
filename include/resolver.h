// A class for resolving variable references
#pragma once
#include "ast.h"
#include "eval.h"

class Resolver {
public:
  enum class FunctionType {
    NONE,
    FUNCTION
  } current_function_type = FunctionType::NONE;
  std::vector<std::unordered_map<std::string, bool>> scopes;
  Evaluator *eval;
  bool had_error_ = false;
  Resolver(Evaluator *eval) : eval(eval){};
  void visit(Stmt *stmt);
  void visit(Expr *expr);
  void visit_var_stmt(Var *var);
  void visit_var_expr(Variable *var_expr);
  void visit_expr_stmt(Expression *expr_stmt);
  void visit_assign(Assign *assign);
  void visit_binary(Binary *b);
  void visit_call(Call *c);
  void visit_grouping(Grouping *g);
  void visit_literal(Literal *l);
  void visit_logical(Logical *l);
  void visit_unary(Unary *u);
  // statements
  void visit_block(Block *block);
  void visit_function(Function *f);
  void visit_if(If *if_stmt);
  void visit_print(Print *prt);
  void visit_return(Return *ret);
  void visit_while(While *wh);
  void begin_scope();
  void end_scope();
  bool resolve(std::vector<Stmt *> stmts);
  void resolve(Stmt *stmt);
  void resolve_local(Expr *e, const std::string &name);
  void resolve(Expr *expr);
  void resolve(Expr *expr, int depth);
  void resolve_function(Function *f, FunctionType type);
  void declare(Token *name);
  void define(Token *name);
};
