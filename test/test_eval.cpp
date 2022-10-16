#include "ast.h"
#include "eval.h"
#include "gtest/gtest.h"

TEST(EvalTest, unary_expr_test_1) {
  // Construct an expression tree
  Literal l;
  l.value = new Token{.token_type_ = FALSE,
                      .literal_string = "false",
                      .literal = new bool(false),
                      .line_no = 0};
  Unary expr;
  expr.op = new Token{.token_type_ = BANG, .literal_string = "!", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_TRUE(obj.type = BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, unary_expr_test_2) {
  // Construct an expression tree
  Literal l;
  l.value = new Token{.token_type_ = TRUE,
                      .literal_string = "true",
                      .literal = new bool(true),
                      .line_no = 0};
  Unary expr;
  expr.op = new Token{.token_type_ = BANG, .literal_string = "!", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_TRUE(obj.type = BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, unary_expr_test_3) {
  // Construct an expression tree
  Literal l;
  l.value = new Token{.token_type_ = NUMBER,
                      .literal_string = "12234.456",
                      .literal = new float(12234.456),
                      .line_no = 0};
  Unary expr;
  expr.op = new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_TRUE(obj.type = FLOAT);
  EXPECT_NEAR((*(float *)obj.val), -12234.456, 6e-5);
}

TEST(EvalTest, unary_expr_test_4) {
  // Construct an expression tree
  Literal l;
  l.value = new Token{.token_type_ = NUMBER,
                      .literal_string = "-12234.456",
                      .literal = new float(-12234.456),
                      .line_no = 0};
  Unary expr;
  expr.op = new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_TRUE(obj.type = FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 12234.456, 6e-5);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
