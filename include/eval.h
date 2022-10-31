#pragma once
#include "ast.h"
#include "env.h"
#include "object.h"

class Evaluator {
private:
  Object visit_unary(Unary *u);
  Object visit_binary(Binary *b);
  Object visit_literal(Literal *l);
  Object visit_grouping(Grouping *g);
  Object visit_variable(Variable *v);
  Object visit_assign(Assign *a);

public:
  Environment env;
  Object eval(Expr *e);
  void eval(std::vector<Stmt *> stmts);
  Object visit(Expr *e);
  void visit(Stmt *s);
};
