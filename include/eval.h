#include "ast.h"

enum ObjectType { STR, FLOAT, BOOL, UNDEFINED };
// A container that can hold a pointer
// to a value and its type
struct Object {
  ObjectType type = UNDEFINED;
  void *val = NULL;
  Object() {};
  Object(ObjectType type, void* val): type(type), val(val) {};
  void copy_data(const Object &o) {
    switch (o.type) {
    case STR:
      type = o.type;
      val = new char[strlen((char *)o.val) + 1];
      strcpy((char *)val, (char *)o.val);
      break;
    case FLOAT:
      type = o.type;
      val = new float(*(float *)o.val);
      break;
    case BOOL:
      type = o.type;
      val = new bool(*(bool *)o.val);
      break;
    default:
      type = UNDEFINED;
      val = NULL;
      break;
    }
  }
  // copy constructor
  Object(const Object &o) { copy_data(o); }

  // assignment operator
  Object &operator=(const Object &o) {
    this->release_data();
    this->copy_data(o);
    return *this;
  }

  // destructor
  ~Object() { release_data(); }

  void release_data() {
    switch (type) {
    case STR:
      delete (char *)val;
      break;
    case FLOAT:
      delete (float *)val;
      break;
    case BOOL:
      delete (bool *)val;
      break;
    default:
      break;
    }
    type = UNDEFINED;
    val = NULL;
  }

  static char *type_to_str(ObjectType type) {
    char *str = new char[6];
    switch (type) {
    case STR:
      snprintf(str, 6, "STR");
      break;
    case FLOAT:
      snprintf(str, 6, "FLOAT");
      break;
    case BOOL:
      snprintf(str, 6, "BOOL");
    default:
      snprintf(str, 6, "UNDEFINED");
      break;
    }
    return str;
  }
};

class Evaluator {
private:
  Object visit_unary(Unary *u);
  Object visit_binary(Binary *b);
  Object visit_literal(Literal *l);

public:
  Object eval(Expr *e);
  Object visit(Expr *e);
};
