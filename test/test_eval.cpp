#include "ast.h"
#include "eval.h"
#include "lox.h"
#include "gtest/gtest.h"

TEST(EvalTest, unary_expr_test_1) {
  // Construct an expression tree
  Literal *l = new Literal();
  l->value = new Token(FALSE, "false", new bool(false), 0);
  Unary expr;
  expr.op = new Token(BANG, "!", nullptr, 0);
  expr.right = l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, unary_expr_test_2) {
  // Construct an expression tree
  Literal *l = new Literal();
  l->value = new Token(TRUE, "true", new bool(true), 0);
  Unary expr;
  expr.op = new Token(BANG, "!", nullptr, 0);
  expr.right = l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, unary_expr_test_3) {
  // Construct an expression tree
  Literal *l = new Literal();
  l->value = new Token(NUMBER, "12234.456", new float(12234.456), 0);
  Unary expr;
  expr.op = new Token(MINUS, "-", nullptr, 0);
  expr.right = l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), -12234.456, 6e-5);
}

TEST(EvalTest, unary_expr_test_4) {
  // Construct an expression tree
  Literal *l = new Literal();
  l->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);
  Unary expr;
  expr.op = new Token(MINUS, "-", nullptr, 0);
  expr.right = l;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 12234.456, 6e-5);
}

TEST(EvalTest, binary_expr_test_5) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "-12233.456", new float(-12233.456), 0);

  Binary expr;
  expr.op = new Token(MINUS, "-", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_6) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "12235.456", new float(12235.456), 0);

  Binary expr;
  expr.op = new Token(PLUS, "+", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_7) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(STRING, "Lovely Dress", strdup("Lovely Dress"), 0);

  Binary expr;
  expr.op = new Token(MINUS, "-", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, UNDEFINED);
}

TEST(EvalTest, binary_expr_test_8) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "-12234.456", new float(-12234.456), 0);

  Binary expr;
  expr.op = new Token(SLASH, "/", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 1, 6e-5);
}

TEST(EvalTest, binary_expr_test_9) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "12", new float(12), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "7", new float(7), 0);

  Binary expr;
  expr.op = new Token(STAR, "*", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, FLOAT);
  EXPECT_NEAR((*(float *)obj.val), 84, 6e-5);
}

TEST(EvalTest, binary_expr_test_10) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "12", new float(12), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "7", new float(7), 0);

  Binary expr;
  expr.op = new Token(GREATER, ">", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_11) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "12", new float(12), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "7", new float(7), 0);

  Binary expr;
  expr.op = new Token(LESS, "<", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_12) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "12", new float(12), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "12", new float(12), 0);

  Binary expr;
  expr.op = new Token(EQUAL_EQUAL, "==", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_13) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "13", new float(13), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "14", new float(14), 0);

  Binary expr;
  expr.op = new Token(GREATER_EQUAL, ">=", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_14) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "13", new float(13), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "14", new float(14), 0);

  Binary expr;
  expr.op = new Token(BANG_EQUAL, "==", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_15) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(NUMBER, "12", new float(12), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(NUMBER, "13", new float(12), 0);

  Binary expr;
  expr.op = new Token(EQUAL_EQUAL, "==", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_16) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0);

  Binary expr;
  expr.op = new Token(EQUAL_EQUAL, "==", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_TRUE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_17) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(STRING, "My Fair Ladies", strdup("My Fair Ladies"), 0);

  Binary expr;
  expr.op = new Token{EQUAL_EQUAL, "==", nullptr, 0};
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, BOOL);
  EXPECT_FALSE((*(bool *)obj.val));
}

TEST(EvalTest, binary_expr_test_18) {
  // Construct an expression tree
  Literal *l1 = new Literal();
  l1->value = new Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0);
  Literal *l2 = new Literal();
  l2->value = new Token(STRING, "My Fair Ladies", strdup("My Fair Ladies"), 0);

  Binary expr;
  expr.op = new Token(GREATER_EQUAL, ">=", nullptr, 0);
  expr.right = l1;
  expr.left = l2;
  // evaluate the tree
  Evaluator eval;
  Object obj = eval.eval(&expr);
  EXPECT_EQ(obj.type, UNDEFINED);
}

TEST(EvalTest, stmt_eval_test_19) {
  Expression *e1 = new Expression();
  {
    Literal *l1 = new Literal();
    l1->value = new Token(STRING, "My Fair Lady", strdup("My Fair Lady"), 0);
    Literal *l2 = new Literal();
    l2->value =
        new Token(STRING, "My Fair Ladies", strdup("My Fair Ladies"), 0);

    Binary *expr = new Binary();
    expr->op = new Token(GREATER_EQUAL, ">=", nullptr, 0);
    expr->right = l1;
    expr->left = l2;
    e1->expression = expr;
  }

  Expression *e2 = new Expression();
  {
    // Construct an expression tree
    Literal *l1 = new Literal();
    l1->value = new Token(NUMBER, "13", new float(13), 0);
    Literal *l2 = new Literal();
    l2->value = new Token(NUMBER, "14", new float(14), 0);

    Binary *expr = new Binary();
    expr->op = new Token(GREATER_EQUAL, ">=", nullptr, 0);
    expr->right = l1;
    expr->left = l2;
    e2->expression = expr;
  }

  Print *p = new Print();
  {
    // Construct an expression tree
    Literal *l1 = new Literal();
    l1->value = new Token(NUMBER, "13", new float(13), 0);
    Literal *l2 = new Literal();
    l2->value = new Token(NUMBER, "14", new float(14), 0);

    Binary *expr = new Binary();
    expr->op = new Token(GREATER_EQUAL, ">=", nullptr, 0);
    expr->right = l1;
    expr->left = l2;
    p->expression = expr;
  }
  std::vector<Stmt *> stmts = {e1, e2, p};
  Evaluator eval;
  eval.eval(stmts);
}

TEST(EvalTest, stmt_eval_test_20) {
  std::string test_code = R"(var a = 1;
                            if ((2>3) or (3<2)) {
                                a = true;
                            } else {
                                a = false;
                            })";
  Lox lox;
  lox.run(test_code);
  Object *o = lox.eval_.env.get(Token(STRING, "a", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
  EXPECT_EQ(Object::object_to_str(*o), "false");
}

TEST(EvalTest, stmt_eval_test_21) {
  std::string test_code = R"(var a = 1;
                            if ((5>3) or (3<2)) {
                                a = true;
                            } else {
                                a = false;
                            })";
  Lox lox;
  lox.run(test_code);
  Object *o = lox.eval_.env.get(Token(STRING, "a", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
  EXPECT_EQ(Object::object_to_str(*o), "true");
}

TEST(EvalTest, stmt_eval_test_22) {
  std::string test_code = R"(var a = 1;
                            if ((5>3) and (3>2)) {
                                a = true;
                            } else {
                                a = false;
                            })";
  Lox lox;
  lox.run(test_code);
  Object *o = lox.eval_.env.get(Token(STRING, "a", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
  EXPECT_EQ(Object::object_to_str(*o), "true");
}

TEST(EvalTest, stmt_eval_test_23) {
  std::string test_code = R"(var a = 1;
                            if ((5>3) and (3<2)) {
                                a = true;
                            } else {
                                a = false;
                            })";
  Lox lox;
  lox.run(test_code);
  Object *o = lox.eval_.env.get(Token(STRING, "a", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
  EXPECT_EQ(Object::object_to_str(*o), "false");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
