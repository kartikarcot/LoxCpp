#include <fstream>
#include <iostream>
#include "spdlog/spdlog.h"
#include "utils.h"


namespace fs = std::filesystem;
std::string read_file_into_string(const std::string &file_path) {
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

void report(const std::string &message, const std::string &where,
                 int line_no) {
  spdlog::error("[line: {0}] Error {1}: {2}", line_no, where, message);
}

void error(const std::string &message, int line_no) {
  report(message, "", line_no);
}


