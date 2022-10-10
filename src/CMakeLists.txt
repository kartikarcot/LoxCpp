add_subdirectory(tools)

add_library(scanner scanner.cpp token.cpp utils.cpp)
target_include_directories(scanner PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(scanner PUBLIC spdlog)

add_library(parser parser.cpp token.cpp utils.cpp)
target_include_directories(parser PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parser PUBLIC spdlog)

add_library(printer printer.cpp)
target_include_directories(printer PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(cpplox main.cpp lox.cpp)
target_include_directories(cpplox PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cpplox PRIVATE spdlog parser scanner printer)
add_dependencies(cpplox ast_file_gen)
install(TARGETS cpplox DESTINATION ${INSTALL_DESTINATION}) 
