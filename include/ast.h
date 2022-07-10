#pragma once
#include "utils.h"
#include "token.h"


class Expr{
  public:
    template <typename T, typename V>
    T accept(V v) {
      return v->visit(this);
    }
    virtual std::string print_type() {
      return "Expr";
    }
};

class Binary : public Expr {
  public:
    Expr* left;
    Token* op;
    Expr* right;
    virtual std::string print_type() {
      return "Binary";
    }
};

class Grouping : public Expr {
  public:
    Expr* expression;
    virtual std::string print_type() {
      return "Grouping";
    }
};

class Literal : public Expr {
  public:
    Token* value;
    virtual std::string print_type() {
      return "Literal";
    }
};

class Unary : public Expr {
  public:
    Token* op;
    Expr* right;
    virtual std::string print_type() {
      return "Unary";
    }
};

