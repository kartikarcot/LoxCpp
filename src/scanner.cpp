#include "scanner.h"
#include "spdlog/spdlog.h"
#include "utils.h"
#include <regex>

const static std::unordered_map<TokenType, std::regex> token_to_regex = {
    {LEFT_PAREN, std::regex("\\(")},
    {RIGHT_PAREN, std::regex("\\)")},
    {LEFT_BRACE, std::regex("\\{")},
    {RIGHT_BRACE, std::regex("\\}")},
    {COMMA, std::regex(",")},
    {DOT, std::regex("\\.")},
    {MINUS, std::regex("-")},
    {PLUS, std::regex("\\+")},
    {SEMICOLON, std::regex(";")},
    {SLASH, std::regex("/")},
    {STAR, std::regex("\\*")},

    // logical and other operators
    {BANG, std::regex("!")},
    {BANG_EQUAL, std::regex("!=")},
    {EQUAL, std::regex("=")},
    {EQUAL_EQUAL, std::regex("==")},
    {GREATER, std::regex(">")},
    {GREATER_EQUAL, std::regex(">=")},
    {LESS, std::regex("<")},
    {LESS_EQUAL, std::regex("<=")},
    {SLASH_SLASH, std::regex("//[^\n]*")},

    // literals
    {IDENTIFIER, std::regex("[_a-zA-Z][_a-zA-Z0-9]*")},
    {STRING, std::regex("\"[^\"]*\"")},
    {NUMBER, std::regex("[0-9]+(\\.[0-9]+)*")},

    // reserved keywords
    {AND, std::regex("and")},
    {CLASS, std::regex("class")},
    {ELSE, std::regex("else")},
    {FALSE, std::regex("false")},
    {FUN, std::regex("fun")},
    {FOR, std::regex("for")},
    {IF, std::regex("if")},
    {NIL, std::regex("nil")},
    {OR, std::regex("or")},
    {PRINT, std::regex("print")},
    {RETURN, std::regex("return")},
    {SUPER, std::regex("super")},
    {THIS, std::regex("this")},
    {TRUE, std::regex("true")},
    {VAR, std::regex("var")},
    {WHILE, std::regex("while")},
};

void static set_value(Token &token) {
  // sets the literal void pointer to the value of the literal
  // it is a number or string
  if (token.token_type_ == NUMBER) {
    token.literal = new float(atof(token.literal_string.c_str()));
  } else if (token.token_type_ == STRING) {
    token.literal = (void *)token.literal_string.c_str();
  } else if (token.token_type_ == FALSE) {
    token.literal = new bool(false);
  } else if (token.token_type_ == TRUE) {
    token.literal = new bool(true);
  }
  return;
}

void Scanner::chew_through_whitespace(size_t &idx) {
  while (idx < source_.size() && std::isspace(source_[idx]))
    idx++;
  return;
}

bool Scanner::parse_token(size_t &idx) {
  char *str_ptr = source_.data() + idx;
  int max_len = 0;
  std::pair<TokenType, int> longest_match;
  bool found = false;
  for (const auto &iter : token_to_regex) {
    std::cmatch match;
    if (std::regex_search(str_ptr, match, iter.second,
                          std::regex_constants::match_continuous)) {
      if (match.length() > max_len) {
        longest_match = {iter.first, match.length()};
        max_len = longest_match.second;
        found = true;
      }
    }
  }
  if (found) {
    // add only non-comment tokens
    if (longest_match.first != TokenType::SLASH_SLASH) {
      auto &token = tokens_.emplace_back();
      token.token_type_ = longest_match.first;
      token.literal_string = std::string(source_, idx, longest_match.second);
      set_value(token);
      idx += longest_match.second;
      spdlog::info("{0} : {1}", token_type_to_str(token.token_type_),
                   token.literal_string);
    } else {
      // commend line go to end
      idx = source_.size();
    }
    return true;
  }
  return false;
}

bool Scanner::scan() {
  // while the tape is not fully consumed
  //  1. chew through all the white space
  //  2. on first non white space character go to read intent mode
  //  3. here read tokens until there is no possible token that can match
  //  4. the last matched token is the value for the latest token
  //  5. If this is end of string then EOF
  //  6. go to step 1
  size_t idx = 0;
  while (true) {
    chew_through_whitespace(idx);
    if (idx >= source_.size())
      break;
    bool ret = parse_token(idx);
    if (!ret) {
      // TODO report error
      report("Code could not be parsed at " + std::to_string(idx), "", 0);
      return false;
    }
    if (idx >= source_.size())
      break;
  }
  return true;
}

void Scanner::init(const std::string &source) {
  source_ = source;
  tokens_.clear();
}
