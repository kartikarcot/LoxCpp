#pragma once
#include "eval.h"
#include "parser.h"
#include "scanner.h"
#include <string>

class Lox {
public:
  bool had_error_ = false;
  Scanner scanner_;
  Parser parser_;
  Evaluator eval_;

  void error(const std::string &message, int line_no);
  void report(const std::string &message, const std::string &where,
              int line_no);
  void run(const std::string &lox_code);
  void run_prompt();
  void run_file(const std::string &file_path);
};
