#pragma once
#include "env.h"
#include "object.h"

class LoxFunction : public Callable {
public:
  const Function *f;
  // TODO(kartikarcot) create a copy of the environment where it was created
  std::shared_ptr<Environment> closure;
  std::string name = "LoxFunction";
  LoxFunction(const Function *f, std::shared_ptr<Environment> closure);
  LoxFunction(const LoxFunction &f) = default;
  virtual Object call(std::vector<Object> args, Evaluator *eval) override;
  virtual int arity() override;
  std::string to_string() { return "<fn " + f->name->literal_string + ">"; }
  virtual Callable *Clone() override { return new LoxFunction(*this); }
  ~LoxFunction();
};
