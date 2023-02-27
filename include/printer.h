#include "ast.h"
#include <string>

class PrettyPrinter {
public:
  std::string paranthesize(const Expr *e);
  std::string visit(const Expr *e);
};
