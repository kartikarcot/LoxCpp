#pragma once

#include "token.h"
#include <vector>

class Scanner {
  std::string source_;
  std::vector<Token> tokens_;
  void chew_through_whitespace(size_t &idx);
  bool parse_token(size_t &idx);
  int current_line_ = 0;

public:
  void init(const std::string &source);
  bool scan();
  std::vector<Token> get_tokens() { return tokens_; }
};
