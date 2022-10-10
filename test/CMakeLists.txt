include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 #release-1.12.1
)

FetchContent_MakeAvailable(googletest)


add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE scanner gtest)
add_test(NAME test_lexer COMMAND test_lexer)
