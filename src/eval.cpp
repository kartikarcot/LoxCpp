#include "eval.h"

Object Evaluator::eval(Expr *e) { return e->accept<Object, Evaluator *>(this); }

Object Evaluator::visit(Expr *e) {
  Binary *b = nullptr;
  b = dynamic_cast<Binary *>(e);
  if (b != nullptr) {
    // TODO perform some computation
	return Object();
  }
  Unary *u = nullptr;
  u = dynamic_cast<Unary *>(e);
  if (u != nullptr) {
    // TODO perform some computation
    return Object();
  }
  Literal *l = nullptr;
  l = dynamic_cast<Literal *>(e);
  if (l != nullptr) {
    // TODO perform some computation
    return Object();
  }
  // Nothing matched we are returning
  return Object();
}
