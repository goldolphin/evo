cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -std=gnu99 -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -std=gnu++11 -Wall -Werror")

include_directories(src/main)

add_library(utils STATIC src/main/utils/memory.h src/main/utils/matcher.h src/main/utils/matcher.c src/main/utils/sbuilder.h src/main/utils/sbuilder.c
        src/main/utils/string.c src/main/utils/string.h src/main/utils/pair.c src/main/utils/pair.h src/main/utils/hashmap.c src/main/utils/hashmap.h
        src/main/utils/stack.h src/main/utils/list.h src/main/utils/vector.c src/main/utils/vector.h)

add_library(lexer STATIC src/main/lexer/character.h src/main/lexer/character.c src/main/lexer/lexer.h src/main/lexer/lexer.c
        src/main/lexer/token.c src/main/lexer/token.h src/main/lexer/token_define.def src/main/lexer/char_stream.c src/main/lexer/char_stream.h src/main/lexer/file_char_stream.c src/main/lexer/file_char_stream.h src/main/lexer/source_info.c src/main/lexer/source_info.h src/main/lexer/basic_char_stream.c src/main/lexer/basic_char_stream.h)
target_link_libraries(lexer utils)

add_library(type STATIC src/main/type/type.c src/main/type/type.h src/main/type/struct.c src/main/type/struct.h src/main/type/fun.c src/main/type/fun.h
        src/main/type/type_table.c src/main/type/type_table.h src/main/type/fun_table.c src/main/type/fun_table.h)
target_link_libraries(type utils)

add_library(parser STATIC src/main/parser/parser.c src/main/parser/parser.h src/main/parser/token_stream.c src/main/parser/token_stream.h
        src/main/parser/ast.c src/main/parser/ast.h src/main/parser/ast_define.def src/main/parser/utils.h
        src/main/parser/module/module.c src/main/parser/module/module.h src/main/parser/module/symbol_table.c src/main/parser/module/symbol_table.h
        src/main/parser/module/operator_table.c src/main/parser/module/operator_table.h src/main/parser/module/var_table.c src/main/parser/module/var_table.h src/main/parser/parser_context.c src/main/parser/parser_context.h)
target_link_libraries(parser lexer type)

add_library(interpreter STATIC src/main/interpreter/environment.c src/main/interpreter/environment.h)
target_link_libraries(interpreter parser)

add_executable(array src/test/c/array.c)
add_executable(list src/test/c/list.c)

add_executable(test-matcher src/test/utils/test_matcher.c)
target_link_libraries(test-matcher utils)

configure_file(src/test/evo/list.evo src/test/evo/list.evo)

add_executable(test-lexer src/test/lexer/test_lexer.c)
target_link_libraries(test-lexer lexer)

add_executable(test-parser src/test/parser/test_parser.c)
target_link_libraries(test-parser parser)

add_executable(test-interpreter src/test/interpreter/test_interpreter.c)
target_link_libraries(test-interpreter interpreter)

enable_testing()
add_test(NAME array COMMAND array)
add_test(NAME list COMMAND list)
add_test(NAME test-matcher COMMAND test-matcher)
add_test(NAME test-lexer COMMAND test-lexer)
add_test(NAME test-parser COMMAND test-parser)
add_test(NAME test-interpreter COMMAND test-interpreter)
