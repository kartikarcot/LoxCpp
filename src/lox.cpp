#include "lox.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include "utils.h"


void Lox::run(const std::string &lox_code) { 
	scanner.init(lox_code);
	scanner.scan();
	spdlog::info("{0}", lox_code); 
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
