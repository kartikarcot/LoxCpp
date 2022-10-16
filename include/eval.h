#include "ast.h"

enum ObjectType { STR, FLOAT, BOOL, UNDEFINED };
// A container that can hold a pointer
// to a value and its type
struct Object {
  ObjectType type = UNDEFINED;
  void *val = NULL;
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
  Object visit_binary(Expr *e);
  Object visit_literal(Literal *l);

public:
  Object eval(Expr *e);
  Object visit(Expr *e);
};
