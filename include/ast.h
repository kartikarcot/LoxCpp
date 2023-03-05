#pragma once
#include "utils.h"
#include "token.h"


class Expr{
  public:
    size_t line_no = 0;
    template <typename T, typename V>
    T accept(V v) const {
      return v->visit(this);
    }
    virtual std::string print_type() {
      return "Expr";
    }
    virtual ~Expr() {} 
};

class Assign : public Expr {
  public:
    std::shared_ptr<Token> name;
    std::shared_ptr<Expr> value;
    virtual std::string print_type() {
      return "Assign";
    }
    
    ~Assign() {
    }
};

class Binary : public Expr {
  public:
    std::shared_ptr<Expr> left;
    std::shared_ptr<Token> op;
    std::shared_ptr<Expr> right;
    virtual std::string print_type() {
      return "Binary";
    }
    
    ~Binary() {
    }
};

class Grouping : public Expr {
  public:
    std::shared_ptr<Expr> expression;
    virtual std::string print_type() {
      return "Grouping";
    }
    
    ~Grouping() {
    }
};

class Literal : public Expr {
  public:
    std::shared_ptr<Token> value;
    virtual std::string print_type() {
      return "Literal";
    }
    
    ~Literal() {
    }
};

class Logical : public Expr {
  public:
    std::shared_ptr<Expr> left;
    std::shared_ptr<Token> op;
    std::shared_ptr<Expr> right;
    virtual std::string print_type() {
      return "Logical";
    }
    
    ~Logical() {
    }
};

class Unary : public Expr {
  public:
    std::shared_ptr<Token> op;
    std::shared_ptr<Expr> right;
    virtual std::string print_type() {
      return "Unary";
    }
    
    ~Unary() {
    }
};

class Call : public Expr {
  public:
    std::shared_ptr<Expr> callee;
    std::shared_ptr<Token> paren;
    std::vector<std::shared_ptr<Expr>> arguments;
    virtual std::string print_type() {
      return "Call";
    }
    
    ~Call() {
    }
};

class Variable : public Expr {
  public:
    std::shared_ptr<Token> name;
    virtual std::string print_type() {
      return "Variable";
    }
    
    ~Variable() {
    }
};

class Stmt{
  public:
    size_t line_no = 0;
    template <typename T, typename V>
    T accept(V v) const {
      return v->visit(this);
    }
    virtual std::string print_type() {
      return "Expr";
    }
    virtual ~Stmt() {} 
};

class Block : public Stmt {
  public:
    std::vector<std::shared_ptr<Stmt>> statements;
    virtual std::string print_type() {
      return "Block";
    }
    
    ~Block() {
    }
};

class Expression : public Stmt {
  public:
    std::shared_ptr<Expr> expression;
    virtual std::string print_type() {
      return "Expression";
    }
    
    ~Expression() {
    }
};

class Print : public Stmt {
  public:
    std::vector<std::shared_ptr<Expr>> expressions;
    virtual std::string print_type() {
      return "Print";
    }
    
    ~Print() {
    }
};

class Var : public Stmt {
  public:
    std::shared_ptr<Token> name;
    std::shared_ptr<Expr> initializer;
    virtual std::string print_type() {
      return "Var";
    }
    
    ~Var() {
    }
};

class If : public Stmt {
  public:
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;
    virtual std::string print_type() {
      return "If";
    }
    
    ~If() {
    }
};

class While : public Stmt {
  public:
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
    virtual std::string print_type() {
      return "While";
    }
    
    ~While() {
    }
};

class Function : public Stmt {
  public:
    std::shared_ptr<Token> name;
    std::vector<std::shared_ptr<Token>> params;
    std::vector<std::shared_ptr<Stmt>> body;
    virtual std::string print_type() {
      return "Function";
    }
    
    ~Function() {
    }
};

class Return : public Stmt {
  public:
    std::shared_ptr<Token> keyword;
    std::shared_ptr<Expr> value;
    virtual std::string print_type() {
      return "Return";
    }
    
    ~Return() {
    }
};

class Class : public Stmt {
  public:
    std::shared_ptr<Token> name;
    std::vector<std::shared_ptr<Stmt>> methods;
    virtual std::string print_type() {
      return "Class";
    }
    
    ~Class() {
    }
};

