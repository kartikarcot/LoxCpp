#pragma once
#include "ast.h"
#include "env.h"
#include "object.h"
#include <memory>

class Evaluator {
public:
  Evaluator();
  std::shared_ptr<Environment> globals;
  std::shared_ptr<Environment> env;
  std::vector<Stmt *> stmts_;
  std::unordered_map<Expr *, int> locals;
  Object visit_unary(Unary *u);
  Object visit_binary(Binary *b);
  Object visit_literal(Literal *l);
  Object visit_grouping(Grouping *g);
  Object visit_variable(Variable *v);
  Object visit_logical(Logical *l);
  Object visit_assign(Assign *a);
  Object visit_call(Call *c);
  Object eval(Expr *e);
  Object *lookup_variable(Token *name, Expr *expr);
  void visit_block(Block *b);
  void visit_if(If *i);
  void visit_while(While *w);
  void eval(std::vector<Stmt *> &&stmts);
  Object visit(Expr *e);
  void visit(Stmt *s);
  void visit_function(Function *f);
  void visit_return(Return *r);
  void execute_block(std::vector<Stmt *> stmts,
                     std::shared_ptr<Environment> env);
  void resolve(Expr *e, int depth);
  ~Evaluator();
};
