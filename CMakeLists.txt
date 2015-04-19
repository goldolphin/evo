cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main/main.c)

add_executable(evo ${SOURCE_FILES})

enable_testing()

add_executable(array src/test/c/array.c)
add_executable(list src/test/c/list.c)

add_test(NAME array COMMAND array)
add_test(NAME list COMMAND list)

