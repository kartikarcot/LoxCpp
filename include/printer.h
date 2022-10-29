#include "ast.h"
#include <string>

class PrettyPrinter {
public:
  std::string paranthesize(Expr *e);
  std::string visit(Expr *e);
};
