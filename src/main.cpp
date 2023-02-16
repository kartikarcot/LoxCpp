#include "stdio.h"
#include <filesystem>

#include "logger.h"
#include "lox.h"

int main(int argc, char **argv) {
  Lox interpreter;
  if (argc == 1) {
    interpreter.run_prompt();
  } else if (argc == 2) {
    std::string file_path = std::string(argv[1]);
    interpreter.run_file(file_path);
  } else {
    CLog::Log(LogLevel::INFO, LogCategory::ALL,
              "The correct usage is cpplox [script] or just cpplox");
    return -1;
  }
  return 0;
}
