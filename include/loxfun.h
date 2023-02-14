#pragma once
#include "object.h"

class LoxFunction : public Callable {
public:
  Function *f;
  LoxFunction(Function *f);
  virtual Object call(std::vector<Object> args, Evaluator *eval) override;
  virtual int arity() override;
  std::string to_string() { return "<fn " + f->name->literal_string + ">"; }
};
