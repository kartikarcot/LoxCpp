#pragma once
#include "token.h"
#include "utils.h"

class Expr {
public:
  template <typename T, typename V> T accept(V v) { return v->visit(this); }
  virtual std::string print_type() { return "Expr"; }
  virtual ~Expr() {}
};

class Assign : public Expr {
public:
  Token *name;
  Expr *value;
  virtual std::string print_type() { return "Assign"; }

  ~Assign() {
    delete name;
    delete value;
  }
};

class Binary : public Expr {
public:
  Expr *left;
  Token *op;
  Expr *right;
  virtual std::string print_type() { return "Binary"; }

  ~Binary() {
    delete left;
    delete op;
    delete right;
  }
};

class Grouping : public Expr {
public:
  Expr *expression;
  virtual std::string print_type() { return "Grouping"; }

  ~Grouping() { delete expression; }
};

class Literal : public Expr {
public:
  Token *value;
  virtual std::string print_type() { return "Literal"; }

  ~Literal() { delete value; }
};

class Unary : public Expr {
public:
  Token *op;
  Expr *right;
  virtual std::string print_type() { return "Unary"; }

  ~Unary() {
    delete op;
    delete right;
  }
};

class Variable : public Expr {
public:
  Token *name;
  virtual std::string print_type() { return "Variable"; }

  ~Variable() { delete name; }
};

class Stmt {
public:
  template <typename T, typename V> T accept(V v) { return v->visit(this); }
  virtual std::string print_type() { return "Expr"; }
  virtual ~Stmt() {}
};

class Block : public Stmt {
public:
  std::vector<Stmt *> statements;
  virtual std::string print_type() { return "Block"; }

  ~Block() {}
};

class Expression : public Stmt {
public:
  Expr *expression;
  virtual std::string print_type() { return "Expression"; }

  ~Expression() { delete expression; }
};

class Print : public Stmt {
public:
  Expr *expression;
  virtual std::string print_type() { return "Print"; }

  ~Print() { delete expression; }
};

class Var : public Stmt {
public:
  Token *name;
  Expr *initializer;
  virtual std::string print_type() { return "Var"; }

  ~Var() {
    delete name;
    delete initializer;
  }
};
