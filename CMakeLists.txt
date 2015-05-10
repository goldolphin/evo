cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Werror")

set(SOURCE_FILES
    src/main/lex/char_stream.c
    src/main/lex/char_stream.h
    src/main/lex/character.c
    src/main/lex/character.h
    src/main/lex/lexer.c
    src/main/lex/lexer.h
    src/main/lex/operators.h
    src/main/lex/source_info.c
    src/main/lex/source_info.h
    src/main/lex/token.c
    src/main/lex/token.h
    src/main/main.c
    src/main/memory/memory.c
    src/main/memory/memory.h)

add_executable(evo ${SOURCE_FILES})

enable_testing()

add_executable(array src/test/c/array.c)
add_executable(list src/test/c/list.c)

add_test(NAME array COMMAND array)
add_test(NAME list COMMAND list)

