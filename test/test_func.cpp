/**
 * @file test_func.cpp
 * Tests for creating and evaluating functions
 */

#include "eval.h"
#include "lox.h"
#include "loxfun.h"
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

TEST(FunctionTest, ParseSimpleFunction) {
  std::string test_code = R"(
                            fun sum(a, b) {
                                return a + b;
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

// A test to parse and check if a function enters the environment
TEST(FunctionTest, EvalFunctionDeclaration) {
  std::string test_code = R"(
                            fun sum(a, b) {
                                return a + b;
                            }
                            var a = 9.0;
                            a = sum(1.0, 2.0);
                            )";
  Scanner scanner;
  scanner.init(test_code);
  scanner.scan();
  Parser parser;
  parser.init(scanner.get_tokens());
  std::vector<Stmt *> stmts = parser.parse_stmts();
  Evaluator eval;
  eval.eval(stmts);
  ASSERT_TRUE(eval.env.get("sum") != nullptr);
  ASSERT_TRUE(eval.env.get("a") != nullptr);
  // assert that the object val pointer is a LoxFunction ptr
  ASSERT_TRUE(eval.env.get("sum")->val != nullptr);
  ASSERT_TRUE(eval.env.get("sum")->type == ObjectType::FUNCTION);
  auto lf_ptr = (LoxFunction *)(eval.env.get("sum")->val);
  ASSERT_TRUE(lf_ptr->arity() == 2);
  // Assert the value of a
  ASSERT_TRUE(eval.env.get("a")->type == ObjectType::FLOAT);
  ASSERT_TRUE(eval.env.get("a")->val != nullptr);
  spdlog::debug("a: {}", *(float *)(eval.env.get("a")->val));
  ASSERT_TRUE(*(float *)(eval.env.get("a")->val) == 3.0);
}

int main(int argc, char **argv) {
  spdlog::cfg::load_env_levels();
  spdlog::set_pattern("%^[%l]%$ %v");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
