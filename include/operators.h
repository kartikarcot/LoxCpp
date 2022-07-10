#pragma once

#include "ast.h"
#include <string>

class PrettyPrinter {
	std::string paranthesize(Expr e) {
		return e.accept(this);
	}
	std::string visit(Expr* e) const {
		return "";
	}
}
