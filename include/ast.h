#pragma once
#include "utils.h"
#include "token.h"


class Expr{
  public:
    template <typename T, typename V>
    T accept(V v) {
      return v->visit(this);
    }
};

class Binary : public Expr {
  public:
    Expr* left;
    Token* op;
    Expr* right;
};

class Grouping : public Expr {
  public:
    Expr* expression;
};

class Literal : public Expr {
  public:
    Token* value;
};

class Unary : public Expr {
  public:
    Token* op;
    Expr* right;
};

