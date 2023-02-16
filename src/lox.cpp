#include "lox.h"
#include "logger.h"
#include "printer.h"
#include "utils.h"
#include <iostream>
#include <sstream>

void Lox::run(const std::string &lox_code) {
  scanner_.init(lox_code);
  scanner_.scan();
  auto tokens = scanner_.get_tokens();
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
