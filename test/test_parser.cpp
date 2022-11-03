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
  EXPECT_TRUE(dynamic_cast<Print *>(stmts[0]) != nullptr);
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
  EXPECT_TRUE(dynamic_cast<Print *>(stmts[0]) != nullptr);
}

TEST(ParserTest, test_parser_5) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(VAR, "", nullptr, 0),
      Token(IDENTIFIER, "foo", nullptr, 0),
      Token(EQUAL, "=", nullptr, 0),
      Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 1);
  EXPECT_TRUE(dynamic_cast<Var *>(stmts[0]) != nullptr);
  EXPECT_TRUE(dynamic_cast<Var *>(stmts[0])->name->literal_string == "foo");
  EXPECT_TRUE(dynamic_cast<Literal *>(
                  dynamic_cast<Var *>(stmts[0])->initializer) != nullptr);
}

TEST(ParserTest, test_parser_6) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(IDENTIFIER, "foo", nullptr, 0),
      Token(EQUAL, "=", nullptr, 0),
      Token(TRUE, "true", new bool(true), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 1);
  EXPECT_TRUE(dynamic_cast<Expression *>(stmts[0]) != nullptr);
  EXPECT_TRUE(
      dynamic_cast<Assign *>(dynamic_cast<Expression *>(stmts[0])->expression)
          ->name->literal_string == "foo");
  EXPECT_TRUE(dynamic_cast<Literal *>(
                  dynamic_cast<Assign *>(
                      dynamic_cast<Expression *>(stmts[0])->expression)
                      ->value)
                  ->value->token_type_ == TRUE);
}

TEST(ParserTest, test_parser_7) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(IDENTIFIER, "foo", nullptr, 0),
      Token(EQUAL, "=", nullptr, 0),
      Token(LEFT_PAREN, "(", nullptr, 0),
      Token(NUMBER, "12", new float(12), 0),
      Token(SLASH, "/", nullptr, 0),
      Token(LEFT_PAREN, "(", nullptr, 0),
      Token(NUMBER, "24", new float(24), 0),
      Token(STAR, "*", nullptr, 0),
      Token(NUMBER, "2", new float(2), 0),
      Token(RIGHT_PAREN, ")", nullptr, 0),
      Token(RIGHT_PAREN, ")", nullptr, 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 1);
  EXPECT_TRUE(stmts[0] != nullptr);
  EXPECT_TRUE(dynamic_cast<Var *>(stmts[0]) != nullptr);
}

TEST(ParserTest, test_parser_8) {
  // clang-format off
  std::vector<Token> tokens = {
      Token(IF, "if", nullptr, 0),
      Token(LEFT_PAREN, "(", nullptr, 0),
      Token(TRUE, "true", nullptr, 0),
      Token(RIGHT_PAREN, ")", new float(12), 0),
      Token(LEFT_BRACE, "{", nullptr, 0),
      Token(NUMBER, "24", new float(24), 0),
      Token(STAR, "*", nullptr, 0),
      Token(NUMBER, "2", new float(2), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(RIGHT_BRACE, "}", nullptr, 0),
      Token(ELSE, "else", nullptr, 0),
      Token(LEFT_BRACE, "{", nullptr, 0),
      Token(NUMBER, "24", new float(24), 0),
      Token(STAR, "*", nullptr, 0),
      Token(NUMBER, "2", new float(2), 0),
      Token(SEMICOLON, ";", nullptr, 0),
      Token(RIGHT_BRACE, "}", nullptr, 0),
      Token(END_OF_FILE, "", nullptr, 0)
  };
  // clang-format on
  Parser p;
  p.init(tokens);
  auto stmts = p.parse_stmts();
  EXPECT_TRUE(stmts.size() == 1);
  EXPECT_TRUE(dynamic_cast<If *>(stmts[0]) != nullptr);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
