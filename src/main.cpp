#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "stdio.h"
#include <filesystem>

#include "lox.h"

int main(int argc, char **argv) {
  spdlog::cfg::load_env_levels();
  spdlog::set_pattern("%^[%l]%$ %v");
  Lox interpreter;
  if (argc == 1) {
    interpreter.run_prompt();
  } else if (argc == 2) {
    std::string file_path = std::string(argv[1]);
    interpreter.run_file(file_path);
  } else {
    spdlog::info("The corrent usage is cpplox [script] or just cpplox");
    return -1;
  }
  return 0;
}
