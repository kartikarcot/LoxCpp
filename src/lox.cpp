#include "lox.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
static std::string read_file_into_string(const std::string &file_path) {
  std::string file_contents;
  if (!fs::exists(file_path)) {
    return "";
  }
  std::ifstream ifs(file_path, std::ifstream::in);
  std::string line;
  std::string contents;
  while (std::getline(ifs, line)) {
    spdlog::info("{0}", line);
    contents = contents + line + "\n";
  }
  return contents;
}

void Lox::error(const std::string &message, int line_no) {
  report(message, "", line_no);
}

void Lox::report(const std::string &message, const std::string &where,
                 int line_no) {
  spdlog::error("[line: {0}] Error {1}: {2}", line_no, where, message);
}
void Lox::run(const std::string &lox_code) { spdlog::info("{0}", lox_code); }

void Lox::run_prompt() {
  std::string line;
  while (true) {
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
