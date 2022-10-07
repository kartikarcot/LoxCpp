add_subdirectory(tools)

add_executable(cpplox main.cpp token.cpp lox.cpp token.cpp scanner.cpp utils.cpp parser.cpp printer.cpp)
target_include_directories(cpplox PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cpplox PRIVATE spdlog)
add_dependencies(cpplox ast_file_gen)
install(TARGETS cpplox DESTINATION ${INSTALL_DESTINATION}) 
