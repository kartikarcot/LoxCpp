#include "parser.h"
#include "gtest/gtest.h"

TEST(ParserTest, test_parser_1) {
  std::vector<Token> tokens = {
      Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0)};
  Parser p;
  p.init(tokens);
  auto expr = p.parse();
  EXPECT_TRUE(expr != nullptr);
}

TEST(ParserTest, test_parser_2) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(BANG, "!", nullptr, 0),
	  Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 3);
}

TEST(ParserTest, test_parser_3) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(PRINT, "", nullptr, 0),
      Token(BANG, "!", nullptr, 0),
	  Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 1);
  EXPECT_TRUE(dynamic_cast<Print*>(stmts[0]) != nullptr);
}

TEST(ParserTest, test_parser_4) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(PRINT, "", nullptr, 0),
      Token(BANG, "!", nullptr, 0),
	  Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
	  Token(PRINT, "", nullptr, 0),
      Token(BANG, "!", nullptr, 0),
	  Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 2);
  EXPECT_TRUE(dynamic_cast<Print*>(stmts[0]) != nullptr);
}
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
