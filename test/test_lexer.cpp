#include "scanner.h"
#include "gtest/gtest.h"

TEST(LexerTest, lexer_test_1) {
  Scanner scanner;
  scanner.init("1+2");
  scanner.scan();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
