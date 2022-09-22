#include <string>
#include "ast.h"

class PrettyPrinter {
public:
  std::string paranthesize(Expr* e); 
  std::string visit(Expr *e); 
};
