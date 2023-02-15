#pragma once
#include "token.h"
#include "utils.h"

class Expr {
public:
  size_t line_no = 0;
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

class Logical : public Expr {
public:
  Expr *left;
  Token *op;
  Expr *right;
  virtual std::string print_type() { return "Logical"; }

  ~Logical() {
    delete left;
    delete op;
    delete right;
  }
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

class Call : public Expr {
public:
  Expr *callee;
  Token *paren;
  std::vector<Expr *> arguments;
  virtual std::string print_type() { return "Call"; }

  ~Call() {
    delete callee;
    delete paren;
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
  size_t line_no = 0;
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
  std::vector<Expr *> expressions;
  virtual std::string print_type() { return "Print"; }

  ~Print() {}
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

class If : public Stmt {
public:
  Expr *condition;
  Stmt *thenBranch;
  Stmt *elseBranch;
  virtual std::string print_type() { return "If"; }

  ~If() {
    delete condition;
    delete thenBranch;
    delete elseBranch;
  }
};

class While : public Stmt {
public:
  Expr *condition;
  Stmt *body;
  virtual std::string print_type() { return "While"; }

  ~While() {
    delete condition;
    delete body;
  }
};

class Function : public Stmt {
public:
  Token *name;
  std::vector<Token *> params;
  std::vector<Stmt *> body;
  virtual std::string print_type() { return "Function"; }

  ~Function() { delete name; }
};

class Return : public Stmt {
public:
  Token *keyword;
  Expr *value;
  virtual std::string print_type() { return "Return"; }

  ~Return() {
    delete keyword;
    delete value;
  }
};
