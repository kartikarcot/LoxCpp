#include "lox.h"
#include "logger.h"
#include "printer.h"
#include "utils.h"
#include <iostream>
#include <sstream>

void Lox::run(const std::string &lox_code) {
  Scanner scanner;
  scanner.init(lox_code);
  scanner.scan();
  auto tokens = scanner.get_tokens();
  Parser parser;
  parser.init(tokens);
  auto stmts = parser.parse_stmts();
  if (stmts.empty())
    return;
  Resolver resolver(&eval_);
  if (!resolver.resolve(stmts)) {
    CLog::FLog(LogLevel::ERROR, LogCategory::ALL, "Resolver failed");
    return;
  }
  eval_.eval(std::move(stmts));
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
