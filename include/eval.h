#pragma once
#include "ast.h"
#include "env.h"
#include "object.h"
#include <memory>

class Evaluator {
public:
  Evaluator();
  std::vector<std::shared_ptr<Stmt>> stmts_;
  std::shared_ptr<Environment> globals;
  std::shared_ptr<Environment> env;
  std::unordered_map<const Expr *, int> locals;
  Object visit_unary(const Unary *u);
  Object visit_binary(const Binary *b);
  Object visit_literal(const Literal *l);
  Object visit_grouping(const Grouping *g);
  Object visit_variable(const Variable *v);
  Object visit_logical(const Logical *l);
  Object visit_assign(const Assign *a);
  Object visit_call(const Call *c);
  Object eval(const Expr *e);
  Object *lookup_variable(const Token *name, const Expr *expr);
  void visit_block(const Block *b);
  void visit_if(const If *i);
  void visit_while(const While *w);
  void eval(std::vector<std::shared_ptr<Stmt>> &&stmts);
  Object visit(const Expr *e);
  void visit(const Stmt *s);
  void visit_function(const Function *f);
  void visit_class(const Class *c);
  void visit_return(const Return *r);
  void execute_block(std::vector<std::shared_ptr<Stmt>> stmts,
                     std::shared_ptr<Environment> env);
  void resolve(const Expr *e, int depth);
  ~Evaluator();
};
