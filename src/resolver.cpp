#include "resolver.h"
#include "ast.h"
#include "logger.h"

void Resolver::visit(const Stmt *stmt) {
  // try to cast this to block
  if (auto block = dynamic_cast<const Block *>(stmt)) {
    visit_block(block);
  }
  if (auto var = dynamic_cast<const Var *>(stmt)) {
    visit_var_stmt(var);
  }
  if (auto assign = dynamic_cast<const Assign *>(stmt)) {
    visit_assign(assign);
  }
  if (auto f = dynamic_cast<const Function *>(stmt)) {
    visit_function(f);
  }
  if (auto c = dynamic_cast<const Class *>(stmt)) {
    visit_class(c);
  }
  if (auto expr = dynamic_cast<const Expression *>(stmt)) {
    visit_expr_stmt(expr);
  }
  if (auto if_st = dynamic_cast<const If *>(stmt)) {
    visit_if(if_st);
  }
  if (auto prt_st = dynamic_cast<const Print *>(stmt)) {
    visit_print(prt_st);
  }
  if (auto ret_st = dynamic_cast<const Return *>(stmt)) {
    visit_return(ret_st);
  }
  if (auto wh_st = dynamic_cast<const While *>(stmt)) {
    visit_while(wh_st);
  }
}

void Resolver::visit(const Expr *expr) {
  if (auto var_expr = dynamic_cast<const Variable *>(expr)) {
    visit_var_expr(var_expr);
  }
  if (auto assign = dynamic_cast<const Assign *>(expr)) {
    visit_assign(assign);
  }
  if (auto binary = dynamic_cast<const Binary *>(expr)) {
    visit_binary(binary);
  }
  if (auto call = dynamic_cast<const Call *>(expr)) {
    visit_call(call);
  }
  if (auto grouping = dynamic_cast<const Grouping *>(expr)) {
    visit_grouping(grouping);
  }
  if (auto literal = dynamic_cast<const Literal *>(expr)) {
    visit_literal(literal);
  }
  if (auto logical = dynamic_cast<const Logical *>(expr)) {
    visit_logical(logical);
  }
  if (auto unary = dynamic_cast<const Unary *>(expr)) {
    visit_unary(unary);
  }
}

void Resolver::visit_block(const Block *block) {
  begin_scope();
  resolve(block->statements);
  end_scope();
}

bool Resolver::resolve(std::vector<std::shared_ptr<Stmt>> stmts) {
  had_error_ = false;
  for (auto stmt : stmts) {
    resolve(stmt.get());
    if (had_error_) {
      return false;
    }
  }
  return true;
}

void Resolver::resolve(const Stmt *stmt) {
  stmt->accept<void, Resolver *>(this);
}

void Resolver::resolve(const Expr *expr) {
  expr->accept<void, Resolver *>(this);
}

void Resolver::begin_scope() { scopes.emplace_back(); }

void Resolver::end_scope() { scopes.pop_back(); }

void Resolver::visit_var_stmt(const Var *var) {
  declare(var->name.get());
  if (var->initializer != nullptr) {
    resolve(var->initializer.get());
  }
  define(var->name.get());
  return;
}

void Resolver::declare(const Token *name) {
  if (scopes.empty())
    return;
  auto &scope = scopes.back();
  if (scope.find(name->literal_string) != scope.end()) {
    report("Variable with this name already declared in this scope.", "",
           name->line_no);
    had_error_ = true;
    return;
  }
  scope[name->literal_string] = false;
}

void Resolver::define(const Token *name) {
  if (scopes.empty())
    return;
  auto &scope = scopes.back();
  scope[name->literal_string] = true;
}

void Resolver::visit_var_expr(const Variable *var_expr) {
  if (!scopes.empty() &&
      scopes.back().find(var_expr->name->literal_string) !=
          scopes.back().end() &&
      scopes.back().at(var_expr->name->literal_string) == false) {
    report("Cannot read local variable in its own initializer.", "",
           var_expr->line_no);
    had_error_ = true;
    return;
  }
  resolve_local(var_expr, var_expr->name->literal_string);
}

void Resolver::resolve_local(const Expr *e, const std::string &name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    if (scopes[i].find(name) != scopes[i].end()) {
      eval->resolve(e, scopes.size() - 1 - i);
      CLog::FLog(LogLevel::DEBUG, LogCategory::RESOLVER,
                 "Resolved local variable %s at depth %zu at %zu", name.c_str(),
                 scopes.size() - 1 - i, (size_t)e);
      return;
    }
  }
}

void Resolver::visit_assign(const Assign *assign) {
  resolve(assign->value.get());
  resolve_local(assign, assign->name->literal_string);
}

void Resolver::visit_function(const Function *f) {
  declare(f->name.get());
  define(f->name.get());
  resolve_function(f, FunctionType::FUNCTION);
  return;
}

void Resolver::resolve_function(const Function *f, FunctionType type) {
  FunctionType enclosing_function_type = current_function_type;
  current_function_type = type;
  begin_scope();
  for (auto param : f->params) {
    declare(param.get());
    define(param.get());
  }
  resolve(f->body);
  end_scope();
}

void Resolver::visit_expr_stmt(const Expression *expr_stmt) {
  resolve(expr_stmt->expression.get());
  return;
}

void Resolver::visit_if(const If *if_st) {
  resolve(if_st->condition.get());
  resolve(if_st->thenBranch.get());
  if (if_st->elseBranch != nullptr) {
    resolve(if_st->elseBranch.get());
  }
  return;
}

void Resolver::visit_print(const Print *prt) {
  for (auto expr : prt->expressions) {
    resolve(expr.get());
  }
  return;
}

void Resolver::visit_return(const Return *ret) {
  if (current_function_type == FunctionType::NONE) {
    report("Cannot return from top-level code.", "", ret->line_no);
    had_error_ = true;
    return;
  }
  if (ret->value != nullptr) {
    resolve(ret->value.get());
  }
}

void Resolver::visit_while(const While *wh) {
  resolve(wh->condition.get());
  resolve(wh->body.get());
}

void Resolver::visit_binary(const Binary *b) {
  resolve(b->left.get());
  resolve(b->right.get());
  return;
}

void Resolver::visit_call(const Call *c) {
  resolve(c->callee.get());
  for (auto arg : c->arguments) {
    resolve(arg.get());
  }
  return;
}

void Resolver::visit_grouping(const Grouping *g) {
  resolve(g->expression.get());
  return;
}

void Resolver::visit_literal(const Literal *l) { return; }

void Resolver::visit_logical(const Logical *l) {
  resolve(l->left.get());
  resolve(l->right.get());
  return;
}

void Resolver::visit_unary(const Unary *u) {
  resolve(u->right.get());
  return;
}

void Resolver::visit_class(const Class *c) {
  // Adds the class to the scope
  declare(c->name.get());
  define(c->name.get());
}
