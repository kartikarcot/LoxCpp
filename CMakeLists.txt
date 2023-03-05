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

# This somehow enables registering of unit tests
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

# copy the template folder to the build directory
# ensure that the templates folder is copied to the build directory
# everytime the project is built
file(COPY templates DESTINATION ${CMAKE_BINARY_DIR})

# copy the server.py to the build directory
file(COPY server.py DESTINATION ${CMAKE_BINARY_DIR})


# build target to start the server.py add dependency to the lox target
# so that the server is started after the lox executable is built and copied
# to the build directory

add_custom_target(
    start_server
    COMMAND cp -r ../templates ./ && cp ../server.py ./ && python3 server.py
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS lox
    COMMENT "Starting server.py"
)
