#include "ast.h"
#include <string>

class PrettyPrinter {
public:
  std::string paranthesize(Expr e) {
    return e.accept<std::string, PrettyPrinter *>(this);
  }
  std::string visit(Expr *e) { 
	  return "";
  }
};

int main(int argc, char **argv) {
  PrettyPrinter p;
  return 0;
}
