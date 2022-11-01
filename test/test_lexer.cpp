#include "scanner.h"
#include "gtest/gtest.h"

TEST(LexerTest, token_test_1) { Token t = Token(NUMBER, "3", new float(3), 0); }

TEST(LexerTest, token_test_2) {
  Token t = Token(NUMBER, "3", new float(3), 10);
  Token t1 = t;
  EXPECT_FLOAT_EQ(3, *(float *)t1.literal);
  EXPECT_FLOAT_EQ(NUMBER, t1.token_type_);
  EXPECT_EQ("3", t1.literal_string);
  EXPECT_EQ(10, t1.line_no);
}

TEST(LexerTest, lexer_test_0) {
  Scanner scanner;
  scanner.init("false");
  scanner.scan();
  auto tokens = scanner.get_tokens();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens[0].token_type_, FALSE);
}

TEST(LexerTest, lexer_test_1) {
  Scanner scanner;
  scanner.init("1+2+3+4+5");
  EXPECT_TRUE(scanner.scan());
  auto tokens = scanner.get_tokens();
  std::vector<TokenType> types = {NUMBER, PLUS,   NUMBER, PLUS,   NUMBER,
                                  PLUS,   NUMBER, PLUS,   NUMBER, END_OF_FILE};
  for (int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].token_type_, types[i]);
  }
  EXPECT_EQ(tokens.size(), 10);
}

TEST(LexerTest, lexer_test_2) {
  Scanner scanner;
  scanner.init("1+2=abs");
  EXPECT_TRUE(scanner.scan());
  EXPECT_EQ(scanner.get_tokens().size(), 6);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
