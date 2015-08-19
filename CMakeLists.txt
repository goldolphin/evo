cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Werror")

include_directories(src/main)

add_library(utils STATIC src/main/utils/memory.h src/main/utils/matcher.h src/main/utils/matcher.c)

add_library(lex STATIC src/main/lex/character.h src/main/lex/character.c src/main/lex/lexer.h src/main/lex/lexer.c)
target_link_libraries(lex utils)

enable_testing()

add_executable(array src/test/c/array.c)
add_executable(list src/test/c/list.c)

add_executable(test-matcher src/test/utils/test_matcher.c)
target_link_libraries(test-matcher utils)

add_test(NAME array COMMAND array)
add_test(NAME list COMMAND list)
add_test(NAME test-matcher COMMAND test-matcher)
