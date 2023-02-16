#include "loxfun.h"
#include "ast.h"
#include "env.h"
#include "eval.h"
#include "logger.h"

LoxFunction::LoxFunction(Function *f, const Environment &closure) {
  this->f = f;
  this->closure = closure;
}

Object LoxFunction::call(std::vector<Object> args, Evaluator *eval) {
  // TODO: Should this be just the globals?
  Environment *local = new Environment(&closure);
  for (size_t i = 0; i < f->params.size(); i++) {
    local->define(f->params[i]->literal_string, args[i]);
  }
  try {
    eval->execute_block(f->body, local);
  } catch (Object ret) {
    CLog::FLog(LogLevel::DEBUG, LogCategory::FUN,
               "Returning from function with value %s",
               Object::object_to_str(ret).c_str());
    return ret;
  }
  // TODO: What about different return types
  return Object();
}

int LoxFunction::arity() { return f->params.size(); }
