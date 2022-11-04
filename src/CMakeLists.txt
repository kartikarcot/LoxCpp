add_subdirectory(tools)

add_library(token token.cpp utils.cpp)
target_include_directories(token PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(token PUBLIC spdlog)

add_library(scanner scanner.cpp utils.cpp)
target_include_directories(scanner PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(scanner PUBLIC spdlog token)

add_library(eval eval.cpp utils.cpp env.cpp)
target_include_directories(eval PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_dependencies(eval ast_file_gen)
target_link_libraries(eval PUBLIC spdlog token)

add_library(parser parser.cpp utils.cpp)
target_include_directories(parser PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_dependencies(parser ast_file_gen)
target_link_libraries(parser PUBLIC spdlog token)

add_library(printer printer.cpp)
target_include_directories(printer PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_dependencies(printer ast_file_gen)
target_link_libraries(printer PUBLIC spdlog)

add_library(lox lox.cpp)
target_include_directories(lox PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_dependencies(lox ast_file_gen)
target_link_libraries(lox PUBLIC spdlog parser scanner printer eval)


add_executable(cpplox main.cpp)
target_include_directories(cpplox PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cpplox PRIVATE spdlog lox)
add_dependencies(cpplox ast_file_gen)
install(TARGETS cpplox DESTINATION ${INSTALL_DESTINATION})
