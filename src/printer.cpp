#include "printer.h"
#include <sstream>
#include <string>

std::string PrettyPrinter::paranthesize(const Expr *e) {
  return e->accept<std::string, PrettyPrinter *>(this);
}

std::string PrettyPrinter::visit(const Expr *e) {
  std::stringstream ss;
  const Binary *b = nullptr;
  b = dynamic_cast<const Binary *>(e);
  if (b != nullptr) {
    ss << "(";
    ss << " " << b->op->literal_string;
    ss << " " << b->left->accept<std::string, PrettyPrinter *>(this);
    ss << " " << b->right->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  const Unary *u = nullptr;
  u = dynamic_cast<const Unary *>(e);
  if (u != nullptr) {
    ss << "(";
    ss << " " << u->op->literal_string;
    ss << " " << u->right->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  const Literal *l = nullptr;
  l = dynamic_cast<const Literal *>(e);
  if (l != nullptr) {
    return l->value->literal_string;
  }
  const Variable *v = nullptr;
  v = dynamic_cast<const Variable *>(e);
  if (v != nullptr) {
    return v->name->literal_string;
  }
  const Call *c = nullptr;
  c = dynamic_cast<const Call *>(e);
  if (c != nullptr) {
    ss << "(";
    ss << " " << c->callee->accept<std::string, PrettyPrinter *>(this);
    for (auto &arg : c->arguments) {
      ss << " " << arg->accept<std::string, PrettyPrinter *>(this);
    }
    ss << ")";
    return ss.str();
  }
  const Logical *l2 = nullptr;
  l2 = dynamic_cast<const Logical *>(e);
  if (l2 != nullptr) {
    ss << "(";
    ss << " " << l2->op->literal_string;
    ss << " " << l2->left->accept<std::string, PrettyPrinter *>(this);
    ss << " " << l2->right->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  const Assign *a = nullptr;
  a = dynamic_cast<const Assign *>(e);
  if (a != nullptr) {
    ss << "( assign ";
    ss << " " << a->name->literal_string;
    ss << " " << a->value->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  return "";
}
