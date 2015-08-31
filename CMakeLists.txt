cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -std=gnu99 -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=gnu++11 -Wall -Werror")

include_directories(src/main)

add_library(utils STATIC src/main/utils/memory.h src/main/utils/matcher.h src/main/utils/matcher.c src/main/utils/sbuilder.h src/main/utils/sbuilder.c)

add_library(lexer STATIC src/main/lexer/character.h src/main/lexer/character.c src/main/lexer/lexer.h src/main/lexer/lexer.c src/main/lexer/token.c src/main/lexer/token.h src/main/lexer/token_define.def src/main/lexer/char_stream.c src/main/lexer/char_stream.h src/main/lexer/file_char_stream.c src/main/lexer/file_char_stream.h)
target_link_libraries(lexer utils)

add_library(parser STATIC src/main/parser/parser.c src/main/parser/parser.h)
target_link_libraries(parser lexer)

add_executable(array src/test/c/array.c src/main/parser/parser.c src/main/parser/parser.h)
add_executable(list src/test/c/list.c)

add_executable(test-matcher src/test/utils/test_matcher.c)
target_link_libraries(test-matcher utils)

configure_file(src/test/evo/list.evo src/test/evo/list.evo)

add_executable(test-lexer src/test/lexer/test_lexer.c)
target_link_libraries(test-lexer lexer)

add_executable(test-parser src/test/parser/test_parser.c)
target_link_libraries(test-parser parser)

enable_testing()
add_test(NAME array COMMAND array)
add_test(NAME list COMMAND list)
add_test(NAME test-matcher COMMAND test-matcher)
