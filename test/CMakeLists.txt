include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 #release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_executable(test_env test_env.cpp)
target_link_libraries(test_env PRIVATE eval gtest spdlog)
target_compile_definitions(test_env PUBLIC UNIT_TEST)
add_test(NAME test_env COMMAND test_env)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PRIVATE parser gtest spdlog)
target_compile_definitions(test_parser PUBLIC UNIT_TEST)
add_test(NAME test_parser COMMAND test_parser)

add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE scanner gtest spdlog)
target_compile_definitions(test_lexer PUBLIC UNIT_TEST)
add_test(NAME test_lexer COMMAND test_lexer)

add_executable(test_eval test_eval.cpp)
target_link_libraries(test_eval PRIVATE eval lox gtest spdlog)
target_compile_definitions(test_lexer PUBLIC UNIT_TEST)
add_test(NAME test_eval COMMAND test_eval)
