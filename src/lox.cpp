#include "lox.h"
#include "printer.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include "eval.h"

void Lox::run(const std::string &lox_code) {
  scanner_.init(lox_code);
  scanner_.scan();
  spdlog::debug("Input string {0}", lox_code);
  auto tokens = scanner_.get_tokens();
  parser_.init(tokens);
  Expr *e = parser_.parse();
  PrettyPrinter p;
  spdlog::info("{0}", p.paranthesize(e));
  Evaluator eval;
  spdlog::info("{0}", Object::object_to_str(eval.eval(e)).c_str());
}

void Lox::run_prompt() {
  std::string line;
  while (true) {
	std::cout<<"> ";
    std::getline(std::cin, line);
    // Ctrl+D signals EOF and break then
    if (std::cin.eof())
      break;
    run(line);
  }
}

void Lox::run_file(const std::string &file_path) {
  std::string contents = read_file_into_string(file_path);
  run(contents);
}
