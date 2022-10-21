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
  EXPECT_EQ(obj.type, BOOL);
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
  EXPECT_EQ(obj.type, BOOL);
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
  expr.op =
      new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
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
  expr.op =
      new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 12234.456, 6e-5);
}

TEST(EvalTest, binary_expr_test_5) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12234.456",
                       .literal = new float(-12234.456),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12233.456",
                       .literal = new float(-12233.456),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_6) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12234.456",
                       .literal = new float(-12234.456),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12235.456",
                       .literal = new float(12235.456),
                       .line_no = 0};

  Binary expr;
  expr.op = new Token{.token_type_ = PLUS, .literal_string = "+", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_7) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12234.456",
                       .literal = new float(-12234.456),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = STRING,
                       .literal_string = "Lovely Dress",
                       .literal = strdup("Lovely Dress"),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{.token_type_ = MINUS, .literal_string = "-", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, UNDEFINED);
}

TEST(EvalTest, binary_expr_test_8) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12234.456",
                       .literal = new float(-12234.456),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "-12234.456",
                       .literal = new float(-12234.456),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{.token_type_ = SLASH, .literal_string = "/", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_9) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "7",
                       .literal = new float(7),
                       .line_no = 0};

  Binary expr;
  expr.op = new Token{.token_type_ = STAR, .literal_string = "*", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 84, 6e-5);
}

TEST(EvalTest, binary_expr_test_10) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "7",
                       .literal = new float(7),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{.token_type_ = GREATER, .literal_string = ">", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_11) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "7",
                       .literal = new float(7),
                       .line_no = 0};

  Binary expr;
  expr.op = new Token{.token_type_ = LESS, .literal_string = "<", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_12) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = EQUAL_EQUAL, .literal_string = "==", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_13) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "13",
                       .literal = new float(13),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "14",
                       .literal = new float(14),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = GREATER_EQUAL, .literal_string = ">=", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_14) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "13",
                       .literal = new float(13),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "14",
                       .literal = new float(14),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{.token_type_ = EQUAL, .literal_string = "=", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 13, 6e-5);
}

TEST(EvalTest, binary_expr_test_15) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "12",
                       .literal = new float(12),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = NUMBER,
                       .literal_string = "13",
                       .literal = new float(12),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = EQUAL_EQUAL, .literal_string = "!=", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_16) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Lady",
                       .literal = strdup("My Fair Lady"),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Lady",
                       .literal = strdup("My Fair Lady"),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = EQUAL_EQUAL, .literal_string = "==", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_17) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Lady",
                       .literal = strdup("My Fair Lady"),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Ladies",
                       .literal = strdup("My Fair Ladies"),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = EQUAL_EQUAL, .literal_string = "==", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_18) {
  // Construct an expression tree
  Literal l1;
  l1.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Lady",
                       .literal = strdup("My Fair Lady"),
                       .line_no = 0};
  Literal l2;
  l2.value = new Token{.token_type_ = STRING,
                       .literal_string = "My Fair Ladies",
                       .literal = strdup("My Fair Ladies"),
                       .line_no = 0};

  Binary expr;
  expr.op =
      new Token{
          .token_type_ = GREATER_EQUAL, .literal_string = ">=", .line_no = 0},
  expr.right = &l1;
  expr.left = &l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, UNDEFINED);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
