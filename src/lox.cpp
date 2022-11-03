#include "lox.h"
#include "printer.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <iostream>
#include <sstream>

void Lox::run(const std::string &lox_code) {
  scanner_.init(lox_code);
  scanner_.scan();
  spdlog::debug("Lox Code:\n{0}", lox_code);
  auto tokens = scanner_.get_tokens();
  spdlog::debug("Scanned tokens {}", tokens.size());
  parser_.init(tokens);
  auto stmts = parser_.parse_stmts();
  eval_.eval(stmts);
}

void Lox::run_prompt() {
  std::string line;
  while (true) {
    std::cout << ">>> ";
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
