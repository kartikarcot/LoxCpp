project(LoxCpp)

cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
)

set(INSTALL_DESTINATION "$ENV{HOME}/.local/bin")

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_SOURCE_DIR}/include/spdlog)

add_subdirectory(src)
add_subdirectory(test)

