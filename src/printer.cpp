#include "ast.h"
#include <sstream>
#include <string>

class PrettyPrinter {
public:
  std::string paranthesize(Expr e) {
    return e.accept<std::string, PrettyPrinter *>(this);
  }
  std::string visit(Expr *e) {
    std::stringstream ss;
    Binary *b = nullptr;
    b = dynamic_cast<Binary *>(e);
    if (b != nullptr) {
      ss << "(";
      ss << " " << b->op->literal_string;
      ss << " " << b->left->accept<std::string, PrettyPrinter *>(this);
      ss << " " << b->right->accept<std::string, PrettyPrinter *>(this);
      ss << ")";
      return ss.str();
    }
    Unary *u = nullptr;
    u = dynamic_cast<Unary *>(e);
    if (u != nullptr) {
      ss << "(";
      ss << " " << b->op->literal_string;
      ss << " " << b->right->accept<std::string, PrettyPrinter *>(this);
      ss << ")";
      return ss.str();
    }
    Literal *l = nullptr;
    l = dynamic_cast<Literal *>(e);
    if (l != nullptr) {
      return l->value->literal_string;
    }
    return "";
  }
};

int main(int argc, char **argv) {
  PrettyPrinter p;
  return 0;
}
