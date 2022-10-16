#include "ast.h"

enum ObjectType {
  STR,
  INT,
  FLOAT
};
// A container that can hold a pointer
// to a value and its type
struct Object {
	ObjectType type;
	void* val;
};

class Evaluator {
public:
  Object eval(Expr *e);
  Object visit(Expr *e);
};
