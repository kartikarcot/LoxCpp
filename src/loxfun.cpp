#include "loxfun.h"
#include "ast.h"
#include "env.h"
#include "eval.h"
#include "logger.h"
#include <memory>

LoxFunction::LoxFunction(Function *f, std::shared_ptr<Environment> closure) {
  this->f = f;
  this->closure = closure;
  CLog::FLog(LogLevel::DEBUG, LogCategory::FUN, "Creating function %s",
             f->name->literal_string.c_str());
  CLog::FLog(LogLevel::DEBUG, LogCategory::FUN,
             "Closure has the following variables:\n%s",
             this->closure->print().c_str());
}

Object LoxFunction::call(std::vector<Object> args, Evaluator *eval) {
  CLog::FLog(LogLevel::DEBUG, LogCategory::FUN, "Calling function %s",
             f->name->literal_string.c_str());
  CLog::FLog(LogLevel::DEBUG, LogCategory::FUN, "With %zu arguments",
             args.size());
  CLog::FLog(LogLevel::DEBUG, LogCategory::FUN,
             "Closure has the following variables:\n%s\nAddress: %zu",
             closure->print().c_str(), (size_t)closure.get());
  auto local = std::make_shared<Environment>(closure.get());
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
  return Object();
}

int LoxFunction::arity() { return f->params.size(); }

LoxFunction::~LoxFunction() {
  // we will delete environs in the interpreter
  // at the block level
}
