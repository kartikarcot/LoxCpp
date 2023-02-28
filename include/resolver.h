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
  void visit(const Stmt *stmt);
  void visit(const Expr *expr);
  void visit_var_stmt(const Var *var);
  void visit_var_expr(const Variable *var_expr);
  void visit_expr_stmt(const Expression *expr_stmt);
  void visit_assign(const Assign *assign);
  void visit_binary(const Binary *b);
  void visit_call(const Call *c);
  void visit_grouping(const Grouping *g);
  void visit_literal(const Literal *l);
  void visit_logical(const Logical *l);
  void visit_unary(const Unary *u);
  // statements
  void visit_block(const Block *block);
  void visit_function(const Function *f);
  void visit_if(const If *if_stmt);
  void visit_print(const Print *prt);
  void visit_return(const Return *ret);
  void visit_while(const While *wh);
  void begin_scope();
  void end_scope();
  bool resolve(std::vector<std::shared_ptr<Stmt>> stmts);
  void resolve(const Stmt *stmt);
  void resolve_local(const Expr *e, const std::string &name);
  void resolve(const Expr *expr);
  void resolve(const Expr *expr, int depth);
  void resolve_function(const Function *f, FunctionType type);
  void declare(const Token *name);
  void define(const Token *name);
};
