project(LoxCpp)

cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 #release-1.12.1
)

FetchContent_MakeAvailable(googletest)

set(INSTALL_DESTINATION "$ENV{HOME}/.local/bin")

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_SOURCE_DIR}/include/spdlog)

# This somehow enables registering of unit tests
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

