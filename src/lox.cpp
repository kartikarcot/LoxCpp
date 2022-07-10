#include "lox.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include "utils.h"
#include <sstream>

class PrettyPrinter {
public:
  std::string paranthesize(Expr* e) {
    return e->accept<std::string, PrettyPrinter *>(this);
  }
  std::string visit(Expr *e) {
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
      ss << " " << b->op->literal_string;
      ss << " " << b->right->accept<std::string, PrettyPrinter *>(this);
      ss << ")";
      return ss.str();
    }
    Literal *l = nullptr;
    l = dynamic_cast<Literal *>(e);
    if (l != nullptr) {
      return l->value->literal_string;
    }
    return "";
  }
};

void Lox::run(const std::string &lox_code) { 
	scanner.init(lox_code);
	scanner.scan();
	spdlog::info("{0}", lox_code); 
	auto tokens = scanner.get_tokens();
	parser.init(tokens);
	Expr* e = parser.parse();
	PrettyPrinter p;
	spdlog::info("{0}", p.paranthesize(e));

}

void Lox::run_prompt() {
  std::string line;
  while (true) {
    std::getline(std::cin, line);
    // Ctrl+D signals EOF and break then
    if (std::cin.eof())
      break;
    run(line);
	spdlog::info(">");
  }
}

void Lox::run_file(const std::string &file_path) {
  std::string contents = read_file_into_string(file_path);
  run(contents);
}
