#include "printer.h"
#include <sstream>
#include <string>

std::string PrettyPrinter::paranthesize(Expr *e) {
  return e->accept<std::string, PrettyPrinter *>(this);
}

std::string PrettyPrinter::visit(Expr *e) {
  std::stringstream ss;
  Binary *b = nullptr;
  // spdlog::info("The type is {0}", ((Binary*)e)->op->literal_string.c_str());
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
    ss << " " << u->op->literal_string;
    ss << " " << u->right->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  Literal *l = nullptr;
  l = dynamic_cast<Literal *>(e);
  if (l != nullptr) {
    return l->value->literal_string;
  }
  Variable *v = nullptr;
  v = dynamic_cast<Variable *>(e);
  if (v != nullptr) {
    return v->name->literal_string;
  }
  Call *c = nullptr;
  c = dynamic_cast<Call *>(e);
  if (c != nullptr) {
    ss << "(";
    ss << " " << c->callee->accept<std::string, PrettyPrinter *>(this);
    for (auto &arg : c->arguments) {
      ss << " " << arg->accept<std::string, PrettyPrinter *>(this);
    }
    ss << ")";
    return ss.str();
  }
  Logical *l2 = nullptr;
  l2 = dynamic_cast<Logical *>(e);
  if (l2 != nullptr) {
    ss << "(";
    ss << " " << l2->op->literal_string;
    ss << " " << l2->left->accept<std::string, PrettyPrinter *>(this);
    ss << " " << l2->right->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  Assign *a = nullptr;
  a = dynamic_cast<Assign *>(e);
  if (a != nullptr) {
    ss << "( assign ";
    ss << " " << a->name->literal_string;
    ss << " " << a->value->accept<std::string, PrettyPrinter *>(this);
    ss << ")";
    return ss.str();
  }
  return "";
}
