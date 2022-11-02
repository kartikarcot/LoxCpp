#include "env.h"
#include "gtest/gtest.h"
#include <string.h>

TEST(TestEnv, test_env_1) {
  Environment env;
  env.define("foo", Object(STR, strdup("foobar")));
  Object *o = env.get(Token(IDENTIFIER, "foo", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
}

TEST(TestEnv, test_env_2) {
  Environment env;
  env.define("foo", Object(STR, strdup("foobar")));
  Environment env1(&env);
  Object *o = env1.get(Token(IDENTIFIER, "foo", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
}

TEST(TestEnv, test_env_3) {
  Environment env;
  env.define("foo", Object(STR, strdup("foobar")));
  Environment env1(&env);
  bool ret = env1.assign("foo", Object(STR, strdup("foobar1")));
  EXPECT_TRUE(ret);
  Object *o = env1.get(Token(IDENTIFIER, "foo", nullptr, 0));
  EXPECT_TRUE(o != nullptr);
  EXPECT_EQ(Object::object_to_str(*o), "foobar1");
}
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
