/**
 * @file test_func.cpp
 * Tests for creating and evaluating functions
 */

#include "eval.h"
#include "lox.h"
#include "gtest/gtest.h"

TEST(FunctionTest, ParseSimpleFunction) {
  std::string test_code = R"(
                            fun sum(a, b) {
                                a + b;
                            }
                            var a = 9.0;
                            )";
  Scanner scanner;
  scanner.init(test_code);
  scanner.scan();
  Parser parser;
  parser.init(scanner.get_tokens());
  std::vector<Stmt *> stmts = parser.parse_stmts();
  // assert that we have two statements
  // and that the first one is a function
  ASSERT_EQ(stmts.size(), 2);
  // check that statment is castable to a Function pointer
  ASSERT_TRUE(dynamic_cast<Function *>(stmts[0]));
  // assert that the num of args is 2
  // and that the body is a block
  // with one statement
  ASSERT_EQ(((Function *)stmts[0])->params.size(), 2);
  ASSERT_EQ(((Function *)stmts[0])->body.size(), 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
