add_executable(ast_generator ast_generator.cpp ${CMAKE_SOURCE_DIR}/src/token.cpp ${CMAKE_SOURCE_DIR}/src/utils.cpp ${CMAKE_SOURCE_DIR}/src/stringbuffer.cpp)
target_link_libraries(ast_generator PRIVATE spdlog)
target_include_directories(ast_generator PRIVATE ${CMAKE_SOURCE_DIR}/include)

# add_custom_target(ast_file_gen COMMAND ast_generator ${CMAKE_SOURCE_DIR}/src/tools/grammar.txt ${CMAKE_SOURCE_DIR}/include/ast.h)
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/include/ast.h 
	COMMAND ast_generator ${CMAKE_SOURCE_DIR}/src/tools/grammar.txt ${CMAKE_SOURCE_DIR}/include/ast.h
	DEPENDS ast_generator ${CMAKE_SOURCE_DIR}/src/tools/grammar.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(ast_file_gen DEPENDS ${CMAKE_SOURCE_DIR}/include/ast.h)
# add_dependencies(ast_generator ast_generator)
