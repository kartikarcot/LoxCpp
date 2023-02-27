#include "eval.h"
#include "logger.h"
#include "loxfun.h"
#include "object.h"
#include "printer.h"
#include "token.h"
#include "utils.h"
#include <cassert>
#include <chrono>
#include <cstddef>
#include <math.h>

Object Evaluator::eval(Expr *e) { return e->accept<Object, Evaluator *>(this); }

// A function to get the current time
double time_now() {
  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  auto value = now_ms.time_since_epoch();
  return value.count();
}

// A callable object that can be used to get the current time
class Clock : public Callable {
public:
  Object call(std::vector<Object> args, Evaluator *) override {
    return Object(FLOAT, new float(time_now()));
  }
  int arity() override { return 0; }
};

Evaluator::Evaluator() {
  globals = std::make_shared<Environment>();
  globals->define("clock", Object(FUNCTION, new Clock()));
  env = globals;
}

// TODO(kartikarcot): take ownwership of these statements
// and release them appropriately
void Evaluator::eval(std::vector<Stmt *> &&stmts) {
  // concatenate stmts_ and stmts
  stmts_.insert(stmts_.end(), stmts.begin(), stmts.end());
  for (const auto &stmt : stmts) {
    if (stmt == nullptr) {
      CLog::FLog(LogLevel::ERROR, LogCategory::EVAL,
                 "Evaluator encountered a nullptr for a statement. Report this "
                 "to askarthikkumar@gmail.com");
      exit(-1);
    }
    stmt->accept<void, Evaluator *>(this);
  }
}

static bool process_minus(const Object &value) {
  // do something
  switch (value.type) {
  case STR: {
    // raise an error
    return false;
  }
  case BOOL: {
    // raise an error
    return false;
  }
  case UNDEFINED: {
    // raise an error
    return false;
  }
  case FLOAT: {
    (*(float *)value.val) = -(*(float *)value.val);
    return true;
  }
  default: {
    return false;
  }
  }
}

static bool is_truthy(const Object &value) {
  switch (value.type) {
  case BOOL: {
    return *((bool *)value.val);
  }
  case UNDEFINED: {
    return false;
  }
  default:
    return true;
  }
}

static inline bool is_same_type(const Object &val1, const Object &val2,
                                const ObjectType *type_assert = NULL) {
  if (!type_assert)
    return val1.type == val2.type;
  else
    return val1.type == val2.type && val1.type == *type_assert;
}

static inline Object handle_plus(const Object &left_val,
                                 const Object &right_val) {
  // for now we handle only the addition of numbers
  ObjectType floaty = FLOAT;
  if (!is_same_type(left_val, right_val, &floaty)) {
    return Object();
  }
  float &value1 = *((float *)left_val.val);
  float &value2 = *((float *)right_val.val);
  return {FLOAT, new float(value1 + value2)};
}

static inline Object handle_minus(const Object &left_val,
                                  const Object &right_val) {
  // for now we handle only the addition of numbers
  ObjectType floaty = FLOAT;
  if (!is_same_type(left_val, right_val, &floaty)) {
    return Object();
  }
  float &value1 = *((float *)left_val.val);
  float &value2 = *((float *)right_val.val);
  return {FLOAT, new float(value1 - value2)};
}

static inline Object handle_star(const Object &left_val,
                                 const Object &right_val) {
  // for now we handle only the addition of numbers
  ObjectType floaty = FLOAT;
  if (!is_same_type(left_val, right_val, &floaty)) {
    return Object();
  }
  float &value1 = *((float *)left_val.val);
  float &value2 = *((float *)right_val.val);
  return {FLOAT, new float(value1 * value2)};
}

static inline Object handle_slash(const Object &left_val,
                                  const Object &right_val) {
  // for now we handle only the addition of numbers
  ObjectType floaty = FLOAT;
  if (!is_same_type(left_val, right_val, &floaty)) {
    return Object();
  }
  float &value1 = *((float *)left_val.val);
  float &value2 = *((float *)right_val.val);
  return {FLOAT, new float(value1 / value2)};
}

static inline Object handle_bang_equal(const Object &left_val,
                                       const Object &right_val) {
  if (!is_same_type(left_val, right_val)) {
    return Object();
  }
  switch (left_val.type) {
  case FLOAT: {
    float &value1 = *((float *)left_val.val);
    float &value2 = *((float *)right_val.val);
    return {FLOAT, new bool(value1 != value2)};
  }
  case STR: {
    char *value1 = ((char *)left_val.val);
    char *value2 = ((char *)right_val.val);
    return {BOOL, new bool(strcmp(value1, value2) != 0)};
  }
  case BOOL: {
    bool &value1 = *((bool *)left_val.val);
    bool &value2 = *((bool *)right_val.val);
    return {BOOL, new bool(value1 != value2)};
  }
  default: {
    return {BOOL, new bool(false)};
  }
  }
}

static inline Object handle_equal_equal(const Object &left_val,
                                        const Object &right_val) {
  if (!is_same_type(left_val, right_val)) {
    return Object();
  }
  switch (left_val.type) {
  case FLOAT: {
    float &value1 = *((float *)left_val.val);
    float &value2 = *((float *)right_val.val);
    return {BOOL, new bool(value1 == value2)};
  }
  case STR: {
    char *value1 = ((char *)left_val.val);
    char *value2 = ((char *)right_val.val);
    return {BOOL, new bool(strcmp(value1, value2) == 0)};
  }
  case BOOL: {
    bool &value1 = *((bool *)left_val.val);
    bool &value2 = *((bool *)right_val.val);
    return {BOOL, new bool(value1 == value2)};
  }
  default: {
    return {BOOL, new bool(false)};
  }
  }
}

static inline float get_value(const Object &val) {
  // this works only on floats and bools
  switch (val.type) {
  case BOOL:
    return float(*((bool *)val.val));
  case FLOAT:
    return *((float *)val.val);
  case STR:
    // TODO: Need to handle this better
    return NAN;
  default:
    return NAN;
  }
}

static inline Object handle_greater(const Object &left_val,
                                    const Object &right_val) {
  bool is_left_num = left_val.type == FLOAT || left_val.type == BOOL;
  bool is_right_num = right_val.type == FLOAT || right_val.type == BOOL;
  if (is_right_num && is_left_num) {
    return {BOOL, new bool(get_value(left_val) > get_value(right_val))};
  } else {
    // comparing undefineds and strings are not allowed
    return Object();
  }
}

static inline Object handle_greater_equal(const Object &left_val,
                                          const Object &right_val) {
  bool is_left_num = left_val.type == FLOAT || left_val.type == BOOL;
  bool is_right_num = right_val.type == FLOAT || right_val.type == BOOL;
  if (is_right_num && is_left_num) {
    return {BOOL, new bool(get_value(left_val) >= get_value(right_val))};
  } else {
    // comparing undefineds and strings are not allowed
    return Object();
  }
}

static inline Object handle_less(const Object &left_val,
                                 const Object &right_val) {
  bool is_left_num = left_val.type == FLOAT || left_val.type == BOOL;
  bool is_right_num = right_val.type == FLOAT || right_val.type == BOOL;
  if (is_right_num && is_left_num) {
    return {BOOL, new bool(get_value(left_val) < get_value(right_val))};
  } else {
    // comparing undefineds and strings are not allowed
    return Object();
  }
}

static inline Object handle_less_equal(const Object &left_val,
                                       const Object &right_val) {
  bool is_left_num = left_val.type == FLOAT || left_val.type == BOOL;
  bool is_right_num = right_val.type == FLOAT || right_val.type == BOOL;
  if (is_right_num && is_left_num) {
    return {BOOL, new bool(get_value(left_val) <= get_value(right_val))};
  } else {
    // comparing undefineds and strings are not allowed
    return Object();
  }
}

Object Evaluator::visit_unary(Unary *u) {
  const Object &value = visit(u->right);
  if (value.type == UNDEFINED) {
    // propagate the undefined upwards
    return value;
  }
  if (u->op->token_type_ == BANG) {
    // do something
    bool new_value = !is_truthy(value);
    return {BOOL, new bool(new_value)};
  } else if (u->op->token_type_ == MINUS) {
    if (!process_minus(value)) {
      error("Could not process the unary operation for '-' operator",
            u->op->line_no);
      // return a nill
      // TODO: maybe we need a better way of handling this
      return Object();
    }
    return value;
  } else {
    // raise an error we should not have reached here
    char *error_str = new char[80];
    snprintf(error_str, 80,
             "Could not process the unary operation for %s with value of the "
             "type %s",
             token_type_to_str(u->op->token_type_).c_str(),
             Object::type_to_str(value.type).c_str());
    error(error_str, u->op->line_no);
  }
  return Object();
}

static bool literal_2_object(Literal *l, Object &obj) {
  switch (l->value->token_type_) {
  case NUMBER: {
    obj.type = FLOAT;
    obj.val = new float(*(float *)l->value->literal);
    return true;
  }
  case STRING: {
    obj.type = STR;
    obj.val = new char[l->value->literal_string.size() + 1];
    strcpy((char *)obj.val, l->value->literal_string.c_str());
    return true;
  }
  case FALSE: {
    obj.type = BOOL;
    obj.val = new bool(false);
    return true;
  }
  case TRUE: {
    obj.type = BOOL;
    obj.val = new bool(true);
    return true;
  }
  default: {
    return false;
  }
  }
  return false;
}

Object Evaluator::visit_literal(Literal *l) {
  Object obj;
  if (!literal_2_object(l, obj)) {
    char error_str[60];
    snprintf(error_str, 60, "Could not evaluate the literal: %s",
             l->value->literal_string.c_str());
    error(error_str, l->value->line_no);
    return Object();
  }
  return obj;
}

Object Evaluator::visit_binary(Binary *b) {
  const Object &left_val = visit(b->left);
  if (left_val.type == UNDEFINED) {
    return left_val;
  }
  const Object &right_val = visit(b->right);
  if (right_val.type == UNDEFINED) {
    return right_val;
  }
  // handle the operators
  switch (b->op->token_type_) {
  case PLUS: {
    return handle_plus(left_val, right_val);
    break;
  }
  case MINUS: {
    return handle_minus(left_val, right_val);
    break;
  }
  case STAR: {
    return handle_star(left_val, right_val);
    break;
  }
  case SLASH: {
    return handle_slash(left_val, right_val);
    break;
  }
  case BANG_EQUAL: {
    return handle_bang_equal(left_val, right_val);
    break;
  }
  case EQUAL_EQUAL: {
    return handle_equal_equal(left_val, right_val);
    break;
  }
  case GREATER: {
    return handle_greater(left_val, right_val);
    break;
  }
  case GREATER_EQUAL: {
    return handle_greater_equal(left_val, right_val);
    break;
  }
  case LESS: {
    return handle_less(left_val, right_val);
    break;
  }
  case LESS_EQUAL: {
    return handle_less_equal(left_val, right_val);
    break;
  }
  default: {
    return Object();
  }
  }
}

Object *Evaluator::lookup_variable(Token *name, Expr *expr) {
  if (locals.find(expr) != locals.end()) {
    int distance = locals[expr];
    CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL,
               "Looking up variable %s at distance %d",
               name->literal_string.c_str(), distance);
    return env->get_at(distance, name->literal_string);
  } else {
    CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL,
               "Looking up variable %s at global scope",
               name->literal_string.c_str());
    return globals->get(name->literal_string);
  }
}

Object Evaluator::visit_variable(Variable *v) {
  Object *obj_ptr = lookup_variable(v->name, v);
  if (obj_ptr != nullptr) {
    return *obj_ptr;
  }
  report("Variable " + v->name->literal_string + " is not defined", "",
         v->name->line_no);

  CLog::FLog(LogLevel::ERROR, LogCategory::EVAL,
             "Lookup failed with Environment:\n%s", this->env->print().c_str());
  return Object();
}

Object Evaluator::visit_assign(Assign *a) {
  const Object &obj = visit(a->value);
  if (obj.type == UNDEFINED) {
    report("Could not evaluate the value of the assignment", "",
           a->value->line_no);
    return obj;
  }
  if (locals.find(a) != locals.end()) {
    int distance = locals[a];
    CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL,
               "[Assign] Looking up variable %s at distance %d",
               a->name->literal_string.c_str(), distance);
    bool ret = env->assign_at(distance, a->name->literal_string, obj);
    return obj;
  } else {
    CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL,
               "[Assign] Looking up variable %s at global scope",
               a->name->literal_string.c_str());
    bool ret = globals->assign(a->name->literal_string, obj);
    return obj;
  }
  report("Variable " + a->name->literal_string +
             " is not defined to be assigned to",
         "", a->name->line_no);
  return Object();
}

Object Evaluator::visit_logical(Logical *l) {
  char error_str[60];
  snprintf(error_str, 60, "Could not evaluate the logical literal: %s",
           l->op->literal_string.c_str());
  Object l_res = visit(l->left);
  if (l_res.type == UNDEFINED) {
    error(error_str, l->op->line_no);
    return l_res;
  }
  bool l_truthy = is_truthy(l_res);
  if (l_truthy && l->op->token_type_ == OR) {
    return Object(BOOL, new bool(true));
  }
  if (!l_truthy && l->op->token_type_ == AND) {
    return Object(BOOL, new bool(false));
  }
  Object r_res = visit(l->right);
  if (r_res.type == UNDEFINED) {
    error(error_str, l->op->line_no);
    return r_res;
  }
  bool r_truthy = is_truthy(r_res);
  if (r_truthy && l->op->token_type_ == OR) {
    return Object(BOOL, new bool(true));
  }
  if (r_truthy && l->op->token_type_ == AND) {
    return Object(BOOL, new bool(true));
  }
  return Object(BOOL, new bool(false));
}

Object Evaluator::visit(Expr *e) {
  PrettyPrinter p;
  CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL, "Visiting Expression: %s",
             p.paranthesize(e).c_str());
  Binary *b = nullptr;
  b = dynamic_cast<Binary *>(e);
  if (b != nullptr) {
    return visit_binary(b);
  }
  Unary *u = nullptr;
  u = dynamic_cast<Unary *>(e);
  if (u != nullptr) {
    return visit_unary(u);
  }
  Literal *l = nullptr;
  l = dynamic_cast<Literal *>(e);
  if (l != nullptr) {
    return visit_literal(l);
  }
  Grouping *g = nullptr;
  g = dynamic_cast<Grouping *>(e);
  if (g != nullptr) {
    return visit(g->expression);
  }
  Variable *v = nullptr;
  v = dynamic_cast<Variable *>(e);
  if (v != nullptr) {
    return visit_variable(v);
  }
  Assign *a = nullptr;
  a = dynamic_cast<Assign *>(e);
  if (a != nullptr) {
    return visit_assign(a);
  }

  Logical *lo = nullptr;
  lo = dynamic_cast<Logical *>(e);
  if (lo != nullptr) {
    return visit_logical(lo);
  }
  Call *c = nullptr;
  c = dynamic_cast<Call *>(e);
  if (c != nullptr) {
    return visit_call(c);
  }
  CLog::FLog(LogLevel::ERROR, LogCategory::EVAL,
             "Could not evaluate the "
             "expression! No rules "
             "matched! Report this error "
             "to askarthikkumar@gmail.com");

  // Nothing matched we are returning
  return Object();
}

void Evaluator::visit_block(Block *b) {
  assert(b != nullptr);
  auto old_env = env;
  {
    this->env = std::make_shared<Environment>(old_env.get());
    for (Stmt *st : b->statements) {
      assert(st != nullptr);
      visit(st);
    }
  }
  this->env = old_env;
}

void Evaluator::visit(Stmt *s) {
  Print *p = nullptr;
  p = dynamic_cast<Print *>(s);
  if (p != nullptr) {
    // construct a string of all the results of the expressions
    std::string result;
    for (Expr *e : p->expressions) {
      result += Object::object_to_str(visit(e));
      result += " ";
    }
    // do something
    printf("%s\n", result.c_str());
    return;
  }
  Expression *e = nullptr;
  e = dynamic_cast<Expression *>(s);
  if (e != nullptr) {
    // do something
    PrettyPrinter p;
    visit(e->expression);
    return;
  }
  Var *v = nullptr;
  v = dynamic_cast<Var *>(s);
  if (v != nullptr) {
    // do something
    const Object &value = visit(v->initializer);
    env->define(v->name->literal_string, std::move(value));
    return;
  }
  Block *b = nullptr;
  b = dynamic_cast<Block *>(s);
  if (b != nullptr) {
    visit_block(b);
    return;
  }
  If *i = nullptr;
  i = dynamic_cast<If *>(s);
  if (i != nullptr) {
    visit_if(i);
    return;
  }
  While *w = nullptr;
  w = dynamic_cast<While *>(s);
  if (w != nullptr) {
    visit_while(w);
    return;
  }
  Function *f = nullptr;
  f = dynamic_cast<Function *>(s);
  if (f != nullptr) {
    visit_function(f);
    return;
  }
  Return *r = nullptr;
  r = dynamic_cast<Return *>(s);
  if (r != nullptr) {
    visit_return(r);
    return;
  }
  // if we reached here then print an error
  CLog::FLog(LogLevel::ERROR, LogCategory::EVAL,
             "Could not evaluate the "
             "statement! No rules "
             "matched! Report this error "
             "to askarthikkumar@gmail.com");
}

void Evaluator::visit_if(If *i) {
  Object o = visit(i->condition);
  if (o.type == UNDEFINED) {
    report("Could not evaluate the condition for the if block", "",
           i->condition->line_no);
    return;
  }
  if (is_truthy(o)) {
    visit(i->thenBranch);
  } else if (i->elseBranch != nullptr) {
    visit(i->elseBranch);
  }
}

void Evaluator::visit_while(While *w) {
  Object o = visit(w->condition);
  if (o.type == UNDEFINED) {
    report("Could not evaluate the expression in the while condition", "",
           w->condition->line_no);
    return;
  }
  while (is_truthy(o)) {
    visit(w->body);
    o = visit(w->condition);
    if (o.type == UNDEFINED) {
      report("Could not evaluate the expression in the while condition", "",
             w->condition->line_no);
      return;
    }
  }
}

Object Evaluator::visit_call(Call *c) {
  // get the function object
  Object callee = visit(c->callee);
  std::vector<Object> args;
  for (Expr *e : c->arguments) {
    Object o = visit(e);
    if (o.type == UNDEFINED) {
      report("Could not evaluate the argument", "", e->line_no);
      return o;
    }
    args.push_back(o);
  }
  if (callee.type != FUNCTION) {
    report("Can only call functions and classes", "", c->paren->line_no);
    return Object();
  }
  if (callee.val == nullptr) {
    report("Function object not defined with a callable. Report to "
           "askarthikkumar@gmail.com",
           "", c->paren->line_no);
    return Object();
  }
  Callable *func = (Callable *)(callee.val);
  if (args.size() != func->arity()) {
    report("Expected " + std::to_string(func->arity()) + " arguments but got " +
               std::to_string(args.size()),
           "", c->paren->line_no);
    return Object();
  }
  return func->call(args, this);
}

void Evaluator::execute_block(std::vector<Stmt *> statements,
                              std::shared_ptr<Environment> clos_env) {
  auto old_env = this->env;
  try {
    this->env = clos_env;
    for (Stmt *s : statements) {
      visit(s);
    }
  } catch (Object &o) {
    CLog::FLog(LogLevel::DEBUG, LogCategory::EVAL,
               "Caught the return object %s", Object::object_to_str(o).c_str());
    this->env = old_env;
    throw o;
  }
  this->env = old_env;
}

void Evaluator::visit_function(Function *f) {
  auto func = new LoxFunction(f, env);
  env->define(f->name->literal_string, Object(FUNCTION, func));
  return;
}

void Evaluator::visit_return(Return *r) {
  Object value;
  if (r->value != nullptr) {
    value = visit(r->value);
  }
  throw value;
}

void Evaluator::resolve(Expr *e, int depth) { locals.insert({e, depth}); }

Evaluator::~Evaluator() {
  // delete all statement objects
  for (Stmt *s : stmts_) {
    delete s;
  }
}
