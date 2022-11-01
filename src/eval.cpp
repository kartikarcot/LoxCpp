#include "eval.h"
#include "spdlog/spdlog.h"
#include "token.h"
#include "utils.h"

Object Evaluator::eval(Expr *e) { return e->accept<Object, Evaluator *>(this); }

void Evaluator::eval(std::vector<Stmt *> stmts) {
  for (const auto &stmt : stmts) {
    if (stmt == nullptr) {
      spdlog::error("Evaluator encountered a nullptr for a statement");
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
  spdlog::info("The value is {0}", value1 - value2);
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
    return {BOOL, new bool(get_value(left_val) < get_value(right_val))};
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
    snprintf((char *)obj.val, l->value->literal_string.size(), "%s",
             l->value->literal_string.c_str());
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

Object Evaluator::visit_variable(Variable *v) {
  Object *obj_ptr = env.get(*v->name);
  if (obj_ptr != nullptr) {
    return *obj_ptr;
  }
  return Object();
}

Object Evaluator::visit_assign(Assign *a) {
  const Object &obj = visit(a->value);
  if (obj.type == UNDEFINED) {
    return obj;
  }
  env.assign(a->name->literal_string, obj);
  return obj;
}

Object Evaluator::visit(Expr *e) {
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
  spdlog::error("Could not evaluate the expression! No rules matched!");

  // Nothing matched we are returning
  return Object();
}

void Evaluator::visit(Stmt *s) {
  Print *p = nullptr;
  p = dynamic_cast<Print *>(s);
  if (p != nullptr) {
    // do something
    auto value = visit(p->expression);
    spdlog::info("{} {}", Object::type_to_str(value.type).c_str(),
                 Object::object_to_str(value).c_str());
    return;
  }
  Expression *e = nullptr;
  e = dynamic_cast<Expression *>(s);
  if (e != nullptr) {
    // do something
    visit(e->expression);
    return;
  }
  Var *v = nullptr;
  v = dynamic_cast<Var *>(s);
  if (v != nullptr) {
    // do something
    const Object &value = visit(v->initializer);
    env.define(v->name->literal_string, std::move(value));
    return;
  }
  report("Could not evaluate the statement", "", 0);
}
