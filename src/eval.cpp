#include "eval.h"
#include "token.h"

Object Evaluator::eval(Expr *e) { return e->accept<Object, Evaluator *>(this); }

static bool process_minus(Object &value) {
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

Object Evaluator::visit_unary(Unary *u) {
  Object value = visit(u->right);
  if (value.type == UNDEFINED) {
    // propagate the undefined upwards
    return value;
  }
  if (u->op->token_type_ == BANG) {
    // do something
    bool new_value = !is_truthy(value);
    return {.type = BOOL, .val = new bool(new_value)};
  } else if (u->op->token_type_ == MINUS) {
    if (!process_minus(value)) {
      // raise an error
      error("Could not process the unary operation for '-' operator",
            u->op->line_no);
      // return a nill
      // TODO: maybe we need a better way of handling this
      return Object();
    }
    return value;
  } else {
    // raise an error we should not have reached here
    char *error_str = new char[65];
    snprintf(error_str, 60,
             "Could not process the unary operation for %s with value of the "
             "type %s",
             token_type_to_str(u->op->token_type_).c_str(),
             Object::type_to_str(value.type));
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
    obj.val = new char[l->value->literal_string.size()];
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

Object Evaluator::visit(Expr *e) {
  Binary *b = nullptr;
  b = dynamic_cast<Binary *>(e);
  if (b != nullptr) {
    // TODO perform some computation
    error("Binary expression evaluation not implemented yet", b->op->line_no);
    return Object();
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
  // Nothing matched we are returning
  return Object();
}
