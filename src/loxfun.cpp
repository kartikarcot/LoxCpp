#include "loxfun.h"
#include "ast.h"
#include "env.h"
#include "eval.h"

LoxFunction::LoxFunction(Function *f) { this->f = f; }

Object LoxFunction::call(std::vector<Object> args, Evaluator *eval) {
  // TODO: Should this be just the globals?
  Environment *local = new Environment(eval->globals);
  for (size_t i = 0; i < f->params.size(); i++) {
    local->define(f->params[i]->literal_string, args[i]);
  }
  eval->execute_block(f->body, local);
  // TODO: What about different return types
  return Object();
}

int LoxFunction::arity() { return f->params.size(); }
