#include "resolver.h"
#include "ast.h"

void Resolver::visit(Stmt *stmt) {
  // try to cast this to block
  if (auto block = dynamic_cast<Block *>(stmt)) {
    visit_block(block);
  }
  if (auto var = dynamic_cast<Var *>(stmt)) {
    visit_var_stmt(var);
  }
  if (auto assign = dynamic_cast<Assign *>(stmt)) {
    visit_assign(assign);
  }
  if (auto f = dynamic_cast<Function *>(stmt)) {
    visit_function(f);
  }
  if (auto expr = dynamic_cast<Expression *>(stmt)) {
    visit_expr_stmt(expr);
  }
  if (auto if_st = dynamic_cast<If *>(stmt)) {
    visit_if(if_st);
  }
  if (auto prt_st = dynamic_cast<Print *>(stmt)) {
    visit_print(prt_st);
  }
  if (auto ret_st = dynamic_cast<Return *>(stmt)) {
    visit_return(ret_st);
  }
  if (auto wh_st = dynamic_cast<While *>(stmt)) {
    visit_while(wh_st);
  }
}

void Resolver::visit(Expr *expr) {
  if (auto var_expr = dynamic_cast<Variable *>(expr)) {
    visit_var_expr(var_expr);
  }
  if (auto assign = dynamic_cast<Assign *>(expr)) {
    visit_assign(assign);
  }
  if (auto binary = dynamic_cast<Binary *>(expr)) {
    visit_binary(binary);
  }
  if (auto call = dynamic_cast<Call *>(expr)) {
    visit_call(call);
  }
  if (auto grouping = dynamic_cast<Grouping *>(expr)) {
    visit_grouping(grouping);
  }
  if (auto literal = dynamic_cast<Literal *>(expr)) {
    visit_literal(literal);
  }
  if (auto logical = dynamic_cast<Logical *>(expr)) {
    visit_logical(logical);
  }
  if (auto unary = dynamic_cast<Unary *>(expr)) {
    visit_unary(unary);
  }
}

void Resolver::visit_block(Block *block) {
  begin_scope();
  resolve(block->statements);
  end_scope();
}

void Resolver::resolve(std::vector<Stmt *> stmts) {
  for (auto stmt : stmts) {
    resolve(stmt);
  }
}

void Resolver::resolve(Stmt *stmt) { stmt->accept<void, Resolver *>(this); }

void Resolver::resolve(Expr *expr) { expr->accept<void, Resolver *>(this); }

void Resolver::begin_scope() { scopes.emplace_back(); }

void Resolver::end_scope() { scopes.pop_back(); }

void Resolver::visit_var_stmt(Var *var) {
  declare(var->name);
  if (var->initializer != nullptr) {
    resolve(var->initializer);
  }
  define(var->name);
  return;
}

void Resolver::declare(Token *name) {
  if (scopes.empty())
    return;
  auto &scope = scopes.back();
  if (scope.find(name->literal_string) != scope.end()) {
    report("Variable with this name already declared in this scope.", "",
           name->line_no);
    return;
  }
  scope[name->literal_string] = false;
}

void Resolver::define(Token *name) {
  if (scopes.empty())
    return;
  auto &scope = scopes.back();
  scope[name->literal_string] = true;
}

void Resolver::visit_var_expr(Variable *var_expr) {
  if (!scopes.empty() &&
      scopes.back().find(var_expr->name->literal_string) !=
          scopes.back().end() &&
      scopes.back().at(var_expr->name->literal_string) == false) {
    report("Cannot read local variable in its own initializer.", "",
           var_expr->line_no);
    return;
  }
  resolve_local(var_expr, var_expr->name->literal_string);
}

void Resolver::resolve_local(Expr *e, const std::string &name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    if (scopes[i].find(name) != scopes[i].end()) {
      eval->resolve(e, scopes.size() - 1 - i);
      return;
    }
  }
}

void Resolver::visit_assign(Assign *assign) {
  resolve(assign->value);
  resolve_local(assign, assign->name->literal_string);
}

void Resolver::visit_function(Function *f) {
  declare(f->name);
  define(f->name);
  resolve_function(f, FunctionType::FUNCTION);
  return;
}

void Resolver::resolve_function(Function *f, FunctionType type) {
  FunctionType enclosing_function_type = current_function_type;
  current_function_type = type;
  begin_scope();
  for (auto param : f->params) {
    declare(param);
    define(param);
  }
  resolve(f->body);
  end_scope();
}

void Resolver::visit_expr_stmt(Expression *expr_stmt) {
  resolve(expr_stmt);
  return;
}

void Resolver::visit_if(If *if_st) {
  resolve(if_st->condition);
  resolve(if_st->thenBranch);
  if (if_st->elseBranch != nullptr) {
    resolve(if_st->elseBranch);
  }
  return;
}

void Resolver::visit_print(Print *prt) {
  for (auto expr : prt->expressions) {
    resolve(expr);
  }
  return;
}

void Resolver::visit_return(Return *ret) {
  if (current_function_type == FunctionType::NONE) {
    report("Cannot return from top-level code.", "", ret->line_no);
    return;
  }
  if (ret->value != nullptr) {
    resolve(ret->value);
  }
}

void Resolver::visit_while(While *wh) {
  resolve(wh->condition);
  resolve(wh->body);
}

void Resolver::visit_binary(Binary *b) {
  resolve(b->left);
  resolve(b->right);
  return;
}

void Resolver::visit_call(Call *c) {
  resolve(c->callee);
  for (auto arg : c->arguments) {
    resolve(arg);
  }
  return;
}

void Resolver::visit_grouping(Grouping *g) {
  resolve(g->expression);
  return;
}

void Resolver::visit_literal(Literal *l) { return; }

void Resolver::visit_logical(Logical *l) {
  resolve(l->left);
  resolve(l->right);
  return;
}

void Resolver::visit_unary(Unary *u) {
  resolve(u->right);
  return;
}
